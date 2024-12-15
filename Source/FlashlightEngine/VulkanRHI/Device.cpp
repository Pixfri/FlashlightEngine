// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#include <FlashlightEngine/VulkanRHI/Device.hpp>

#include <FlashlightEngine/Core/Logger.hpp>

#include <GLFW/glfw3.h>

#include <set>

namespace FlashlightEngine::Vk {
    Device::Device(const std::shared_ptr<Instance>& instance, const std::shared_ptr<Surface>& surface)
        : m_Instance(instance), m_Surface(surface) {
        PickPhysicalDevice();
        CreateDevice();
        CreateAllocator();
    }

    Device::~Device() {
        WaitIdle();

        if (m_Allocator != VK_NULL_HANDLE) {
            vmaDestroyAllocator(m_Allocator);
        }

        if (m_Device != VK_NULL_HANDLE) {
            vkDestroyDevice(m_Device, nullptr);
        }
    }

    void Device::PickPhysicalDevice() {
        UInt32 deviceCount = 0;
        vkEnumeratePhysicalDevices(m_Instance->GetHandle(), &deviceCount, nullptr);
        if (deviceCount == 0) {
            Log::Error("[Vulkan] Failed to find GPUs with Vulkan support!");
            throw std::runtime_error("Failed to find GPUs with Vulkan support!");
        }

        Log::Debug("[Vulkan] Device count: {}", deviceCount);

        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(m_Instance->GetHandle(), &deviceCount, devices.data());

        for (const auto& device : devices) {
            if (IsDeviceSuitable(device)) {
                m_PhysicalDevice = device;
                break;
            }
        }

        if (m_PhysicalDevice == VK_NULL_HANDLE) {
            Log::Error("[Vulkan] Failed to find a suitable GPU!");
            throw std::runtime_error("Failed to find a suitable GPU!");
        }

        VkPhysicalDeviceProperties properties;
        vkGetPhysicalDeviceProperties(m_PhysicalDevice, &properties);

        Log::Info("[Vulkan] Selected GPU: {}", properties.deviceName);
    }

    void Device::CreateDevice() {
        const QueueFamilyIndices indices = FindQueueFamilies(m_PhysicalDevice);

        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::set uniqueQueueFamilies = {indices.GraphicsFamily, indices.PresentFamily};

        constexpr float queuePriority = 1.0f;
        for (UInt32 queueFamily : uniqueQueueFamilies) {
            VkDeviceQueueCreateInfo queueCreateInfo = {};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamily;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;
            queueCreateInfos.push_back(queueCreateInfo);
        }

        VkPhysicalDeviceFeatures deviceFeatures = {};
        deviceFeatures.samplerAnisotropy = VK_TRUE;

        VkDeviceCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

        createInfo.queueCreateInfoCount = static_cast<UInt32>(queueCreateInfos.size());
        createInfo.pQueueCreateInfos = queueCreateInfos.data();

        createInfo.pEnabledFeatures = &deviceFeatures;
        createInfo.enabledExtensionCount = static_cast<UInt32>(m_DeviceExtensions.size());
        createInfo.ppEnabledExtensionNames = m_DeviceExtensions.data();

        if (vkCreateDevice(m_PhysicalDevice, &createInfo, nullptr, &m_Device) != VK_SUCCESS) {
            Log::Error("[Vulkan] Failed to create logical device!");
            throw std::runtime_error("Failed to create logical device!");
        }

        vkGetDeviceQueue(m_Device, indices.GraphicsFamily, 0, &m_GraphicsQueue);
        vkGetDeviceQueue(m_Device, indices.PresentFamily, 0, &m_PresentQueue);
        vkGetDeviceQueue(m_Device, indices.TransferFamily, 0, &m_TransferQueue);
        vkGetDeviceQueue(m_Device, indices.ComputeFamily, 0, &m_ComputeQueue);
    }

    void Device::CreateAllocator() {
        VmaAllocatorCreateInfo allocatorInfo = {};
        allocatorInfo.physicalDevice = m_PhysicalDevice;
        allocatorInfo.device = m_Device;
        allocatorInfo.instance = m_Instance->GetHandle();
        allocatorInfo.vulkanApiVersion = VK_API_VERSION_1_3;

        if (vmaCreateAllocator(&allocatorInfo, &m_Allocator) != VK_SUCCESS) {
            Log::Error("[Vulkan] Failed to create allocator!");
            throw std::runtime_error("Failed to create allocator!");
        }
    }

    bool Device::IsDeviceSuitable(VkPhysicalDevice device) const {
        const QueueFamilyIndices indices = FindQueueFamilies(device);

        const bool extensionsSupported = CheckDeviceExtensionSupport(device);

        bool swapChainAdequate = false;
        if (extensionsSupported) {
            SwapchainSupportDetails swapchainSupport = QuerySwapchainSupport(device);
            swapChainAdequate = !swapchainSupport.formats.empty() && !swapchainSupport.presentModes.empty();
        }

        VkPhysicalDeviceFeatures supportedFeatures;
        vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

        return indices.IsComplete() && extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy;
    }

    QueueFamilyIndices Device::FindQueueFamilies(VkPhysicalDevice device) const {
        QueueFamilyIndices indices;

        UInt32 queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

        Int32 i = 0;
        for (const auto& queueFamily : queueFamilies) {
            if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                indices.GraphicsFamily = i;
                indices.GraphicsFamilyFound = true;
            }

            if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_TRANSFER_BIT) {
                indices.TransferFamily = i;
                indices.TransferFamilyFound = true;
            }

            if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT) {
                indices.ComputeFamily = i;
                indices.ComputeFamilyFound = true;
            }

            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, m_Surface->GetHandle(), &presentSupport);
            if (queueFamily.queueCount > 0 && presentSupport) {
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

    bool Device::CheckDeviceExtensionSupport(VkPhysicalDevice device) const {
        UInt32 extensionCount = 0;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

        std::set<std::string> required(m_DeviceExtensions.begin(), m_DeviceExtensions.end());

        for (const auto& [extensionName, specVersion] : availableExtensions) {
            required.erase(extensionName);
        }

        return required.empty();
    }

    SwapchainSupportDetails Device::QuerySwapchainSupport(VkPhysicalDevice device) const {
        SwapchainSupportDetails details;
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, m_Surface->GetHandle(), &details.capabilities);

        UInt32 formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_Surface->GetHandle(), &formatCount, nullptr);

        if (formatCount != 0) {
            details.formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_Surface->GetHandle(), &formatCount, details.formats.data());
        }

        UInt32 presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_Surface->GetHandle(), &presentModeCount, nullptr);

        if (presentModeCount != 0) {
            details.presentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_Surface->GetHandle(), &presentModeCount,
                                                      details.presentModes.data());
        }

        return details;
    }
}
