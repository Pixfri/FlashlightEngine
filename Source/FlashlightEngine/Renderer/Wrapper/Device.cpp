// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#include <FlashlightEngine/Renderer/Wrapper/Device.hpp>

#include <FlashlightEngine/Renderer/Wrapper/VulkanUtils.hpp>

namespace FlashlightEngine {
    Device::Device(const Instance& instance, const std::shared_ptr<Surface>& surface)
        : m_Surface(surface) {
        PickPhysicalDevice(instance);
    }

    Device::~Device() = default;

    Device::Device(Device&& other) noexcept {
        m_PhysicalDevice = other.m_PhysicalDevice;
        other.m_PhysicalDevice = VK_NULL_HANDLE;

        m_Surface = other.m_Surface;
        other.m_Surface = nullptr;
    }

    Device& Device::operator=(Device&& other) noexcept {
        m_PhysicalDevice = other.m_PhysicalDevice;
        other.m_PhysicalDevice = VK_NULL_HANDLE;

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
        spdlog::info("\t- Name: {}", properties.deviceName);
        spdlog::info("\t- Type: {}", VkPhysicalDeviceTypeToString(properties.deviceType));
        spdlog::info("\t- Vulkan API version: {}.{}.{}",
                     VK_API_VERSION_MAJOR(properties.apiVersion),
                     VK_API_VERSION_MINOR(properties.apiVersion),
                     VK_API_VERSION_PATCH(properties.apiVersion));

        // NVIDIA
        if (properties.vendorID == 4318) {
            spdlog::info("\t- Driver version: {}.{}.{}.{}",
                         (properties.driverVersion >> 22) & 0x3FF,
                         (properties.driverVersion >> 14) & 0x0FF,
                         (properties.driverVersion >> 6) & 0x0FF,
                         (properties.driverVersion) & 0x003F);
        }
#if defined(_WIN32) || defined(_WIN64)
        // Intel, only differs on Windows.
        else if (properties.vendorID == 0x8086) {
            spdlog::info("\t- Driver version: {}.{}",
                         (properties.driverVersion >> 14),
                         (properties.driverVersion) & 0x3FFF);
        }
#endif
        // Use Vulkan macros for other vendors
        else {
            spdlog::info("\t- Driver version: {}.{}.{}",
                         VK_API_VERSION_MAJOR(properties.driverVersion),
                         VK_API_VERSION_MINOR(properties.driverVersion),
                         VK_API_VERSION_PATCH(properties.driverVersion));
        }
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

        QueueFamilyIndices indices = FindQueueFamilies(device);

        return features12.bufferDeviceAddress &&
            features12.descriptorIndexing &&
            features13.dynamicRendering &&
            features13.synchronization2 &&
            indices.IsComplete();
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
                indices.PresentFamily = i;
                indices.PresentFamilyFound = true;
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
}
