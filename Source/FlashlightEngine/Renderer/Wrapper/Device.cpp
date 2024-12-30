// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#include <FlashlightEngine/Renderer/Wrapper/Device.hpp>

#include <FlashlightEngine/Renderer/Wrapper/VulkanUtils.hpp>

#include <set>
#include <unordered_set>

namespace FlashlightEngine {
    Device::Device(const Instance& instance, const std::shared_ptr<Surface>& surface, const bool enableValidationLayers)
        : m_Surface(surface) {
        PickPhysicalDevice(instance);
        CreateDevice(instance, enableValidationLayers);
    }

    Device::~Device() {
        if (IsValid()) {
            vkDestroyDevice(m_Device, nullptr);
            spdlog::debug("[Vulkan] Device destroyed.");
        }
    }

    Device::Device(Device&& other) noexcept {
        m_PhysicalDevice = other.m_PhysicalDevice;
        other.m_PhysicalDevice = VK_NULL_HANDLE;

        m_Device = other.m_Device;
        other.m_Device = VK_NULL_HANDLE;

        m_Surface = other.m_Surface;
        other.m_Surface = nullptr;
    }

    Device& Device::operator=(Device&& other) noexcept {
        m_PhysicalDevice = other.m_PhysicalDevice;
        other.m_PhysicalDevice = VK_NULL_HANDLE;

        m_Device = other.m_Device;
        other.m_Device = VK_NULL_HANDLE;

        m_Surface = other.m_Surface;
        other.m_Surface = nullptr;

        return *this;
    }

    void Device::PickPhysicalDevice(const Instance& instance) {
        UInt32 deviceCount = 0;
        vkEnumeratePhysicalDevices(instance.GetInstance(), &deviceCount, nullptr);

        if (deviceCount == 0) {
            spdlog::error("[Vulkan] Failed to find a GPU with Vulkan support.");
            throw std::runtime_error("Failed to find a GPU with Vulkan support.");
        }

        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(instance.GetInstance(), &deviceCount, devices.data());

        for (const auto& device : devices) {
            if (IsDeviceSuitable(device)) {
                m_PhysicalDevice = device;
                break;
            }
        }

        if (m_PhysicalDevice == VK_NULL_HANDLE) {
            spdlog::error("[Vulkan] Failed to find a GPU with the required features.");
            throw std::runtime_error("Failed to find a GPU with the required features.");
        }

        VkPhysicalDeviceProperties properties = GetPhysicalDeviceProperties();

        spdlog::info("[Vulkan] Chosen physical device:");
        spdlog::info("[Vulkan] \t- Name: {}", properties.deviceName);
        spdlog::info("[Vulkan] \t- Type: {}", VkPhysicalDeviceTypeToString(properties.deviceType));
        spdlog::info("[Vulkan] \t- Vulkan API version: {}.{}.{}",
                     VK_API_VERSION_MAJOR(properties.apiVersion),
                     VK_API_VERSION_MINOR(properties.apiVersion),
                     VK_API_VERSION_PATCH(properties.apiVersion));

        // NVIDIA
        if (properties.vendorID == 4318) {
            spdlog::info("[Vulkan] \t- Driver version: {}.{}.{}.{}",
                         (properties.driverVersion >> 22) & 0x3FF,
                         (properties.driverVersion >> 14) & 0x0FF,
                         (properties.driverVersion >> 6) & 0x0FF,
                         (properties.driverVersion) & 0x003F);
        }
#if defined(_WIN32) || defined(_WIN64)
        // Intel, only differs on Windows.
        else if (properties.vendorID == 0x8086) {
            spdlog::info("[Vulkan] \t- Driver version: {}.{}",
                         (properties.driverVersion >> 14),
                         (properties.driverVersion) & 0x3FFF);
        }
#endif
        // Use Vulkan macros for other vendors
        else {
            spdlog::info("[Vulkan] \t- Driver version: {}.{}.{}",
                         VK_API_VERSION_MAJOR(properties.driverVersion),
                         VK_API_VERSION_MINOR(properties.driverVersion),
                         VK_API_VERSION_PATCH(properties.driverVersion));
        }
    }

    void Device::CreateDevice(const Instance& instance, const bool enableValidationLayers) {
        const QueueFamilyIndices indices = GetQueueFamilies();

        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::set<UInt32> uniqueQueueFamilies = {
            indices.GraphicsFamily,
            indices.PresentFamily,
            indices.TransferFamily,
            indices.ComputeFamily
        };

        constexpr Float32 queuePriority = 1.0f;
        for (const UInt32 queueFamily : uniqueQueueFamilies) {
            VkDeviceQueueCreateInfo queueCreateInfo{};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamily;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;
            queueCreateInfos.push_back(queueCreateInfo);
        }

        VkDeviceCreateInfo deviceInfo{};
        deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

        deviceInfo.queueCreateInfoCount = static_cast<UInt32>(queueCreateInfos.size());
        deviceInfo.pQueueCreateInfos = queueCreateInfos.data();

        // Build the pNext chain to enable Vulkan 1.2 and 1.3 core features.
        VkPhysicalDeviceFeatures2 features2{};
        features2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;

        VkPhysicalDeviceVulkan12Features features12{};
        features12.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES;
        features12.descriptorIndexing = true;
        features12.bufferDeviceAddress = true;

        VkPhysicalDeviceVulkan13Features features13{};
        features13.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES;
        features13.dynamicRendering = true;
        features13.synchronization2 = true;

        features2.pNext = &features12;
        features12.pNext = &features13;

        deviceInfo.pNext = &features2;

        deviceInfo.enabledExtensionCount = static_cast<UInt32>(m_RequiredExtensions.size());
        deviceInfo.ppEnabledExtensionNames = m_RequiredExtensions.data();

#if defined(FL_DEBUG) || defined(FL_FORCE_VULKAN_DEBUG)
        // Previous Vulkan implementations made the difference between device and instance specific layers, but this
        // is no longer the case. It is still a good practice to specify them.
        auto enabledValidationLayers = instance.GetEnabledValidationLayers();
        if (enableValidationLayers) {
            deviceInfo.enabledLayerCount = static_cast<UInt32>(enabledValidationLayers.size());
            deviceInfo.ppEnabledLayerNames = enabledValidationLayers.data();
        } else {
            deviceInfo.enabledLayerCount = 0;
            deviceInfo.ppEnabledLayerNames = nullptr;
        }
#else
        deviceInfo.enabledLayerCount = 0;
        deviceInfo.ppEnabledLayerNames = nullptr;
#endif

        if (const VkResult result = vkCreateDevice(m_PhysicalDevice, &deviceInfo, nullptr, &m_Device);
            result != VK_SUCCESS) {
            spdlog::error("[Vulkan] Failed to create device. Error: {}", VkResultToString(result));
            throw std::runtime_error("[Vulkan] Failed to create device.");
        }

        vkGetDeviceQueue(m_Device, indices.GraphicsFamily, 0, &m_GraphicsQueue);
        vkGetDeviceQueue(m_Device, indices.PresentFamily, 0, &m_PresentQueue);
        vkGetDeviceQueue(m_Device, indices.TransferFamily, 0, &m_TransferQueue);
        vkGetDeviceQueue(m_Device, indices.ComputeFamily, 0, &m_ComputeQueue);

        spdlog::debug("[Vulkan] Successfully created device.");
    }

    bool Device::IsDeviceSuitable(VkPhysicalDevice device) const {
        VkPhysicalDeviceProperties properties{};
        VkPhysicalDeviceFeatures2 features{};
        features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;

        VkPhysicalDeviceVulkan12Features features12{};
        features12.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES;

        VkPhysicalDeviceVulkan13Features features13{};
        features13.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES;

        features.pNext = &features12;
        features12.pNext = &features13;

        vkGetPhysicalDeviceProperties(device, &properties);
        vkGetPhysicalDeviceFeatures2(device, &features);

        bool featuresSupported = features12.bufferDeviceAddress &&
            features12.descriptorIndexing &&
            features13.dynamicRendering &&
            features13.synchronization2;

        QueueFamilyIndices indices = FindQueueFamilies(device);

        bool extensionsSupported = CheckExtensionsSupport(device);

        bool swapchainAdequate = false;
        if (extensionsSupported) {
            SwapchainSupportDetails swapchainSupport = QuerySwapchainSupport(device);
            swapchainAdequate = !swapchainSupport.Formats.empty() && !swapchainSupport.PresentModes.empty();
        }

        return featuresSupported && indices.IsComplete() && extensionsSupported && swapchainAdequate;
    }

    QueueFamilyIndices Device::FindQueueFamilies(VkPhysicalDevice device) const {
        QueueFamilyIndices indices{};

        UInt32 queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

        UInt32 i = 0;
        for (const auto& queueFamily : queueFamilies) {
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                indices.GraphicsFamily = i;
                indices.GraphicsFamilyFound = true;
            }

            if (queueFamily.queueFlags & VK_QUEUE_TRANSFER_BIT) {
                indices.TransferFamily = i;
                indices.TransferFamilyFound = true;
            }

            if (queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT) {
                indices.ComputeFamily = i;
                indices.ComputeFamilyFound = true;
            }

            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, m_Surface->GetSurface(), &presentSupport);

            if (presentSupport) {
                indices.PresentFamily = i;
                indices.PresentFamilyFound = true;
            }

            if (indices.IsComplete()) {
                break;
            }

            i++;
        }

        return indices;
    }

    SwapchainSupportDetails Device::QuerySwapchainSupport(VkPhysicalDevice device) const {
        SwapchainSupportDetails details{};

        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, m_Surface->GetSurface(), &details.Capabilities);

        UInt32 formatCount = 0;
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_Surface->GetSurface(), &formatCount, nullptr);

        if (formatCount != 0) {
            details.Formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_Surface->GetSurface(), &formatCount,
                                                 details.Formats.data());
        }

        UInt32 presentModeCount = 0;
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_Surface->GetSurface(), &presentModeCount,
                                                  nullptr);

        if (presentModeCount != 0) {
            details.PresentModes.resize(formatCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_Surface->GetSurface(), &formatCount,
                                                      details.PresentModes.data());
        }

        return details;
    }

    bool Device::CheckExtensionsSupport(VkPhysicalDevice device) const {
        UInt32 availableExtensionCount = 0;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &availableExtensionCount, nullptr);

        std::vector<VkExtensionProperties> availableExtensions(availableExtensionCount);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &availableExtensionCount, availableExtensions.data());

        VkPhysicalDeviceProperties properties;
        vkGetPhysicalDeviceProperties(device, &properties);

        spdlog::debug("[Vulkan] Available device extensions for {}:", properties.deviceName);

        for (const auto& [extensionName, specVersion] : availableExtensions) {
            spdlog::debug("[Vulkan] \t- {}", extensionName);
        }

        std::set<std::string> requiredExtensions(m_RequiredExtensions.begin(), m_RequiredExtensions.end());

        spdlog::debug("[Vulkan] Required device extensions:");
        for (const auto& extension : requiredExtensions) {
            spdlog::debug("[Vulkan] \t- {}", extension);
        }

        for (const auto& [extensionName, specVersion] : availableExtensions) {
            requiredExtensions.erase(extensionName);
        }

        return requiredExtensions.empty();
    }
}
