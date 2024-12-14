// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

namespace FlashlightEngine::Vk {
    inline VkPhysicalDevice Device::GetPhysicalDevice() const {
        return m_PhysicalDevice;
    }

    inline VkDevice Device::GetDevice() const {
        return m_Device;
    }

    inline VkQueue Device::GetGraphicsQueue() const {
        return m_GraphicsQueue;
    }

    inline VkQueue Device::GetPresentQueue() const {
        return m_PresentQueue;
    }

    inline SwapchainSupportDetails Device::GetSwapchainSupport() const {
        return QuerySwapchainSupport(m_PhysicalDevice);
    }

    inline QueueFamilyIndices Device::GetQueueIndices() const {
        return FindQueueFamilies(m_PhysicalDevice);
    }

    inline VkPhysicalDeviceProperties Device::GetPhysicalDeviceProperties() const {
        VkPhysicalDeviceProperties properties;
        vkGetPhysicalDeviceProperties(m_PhysicalDevice, &properties);

        return properties;
    }

    inline void Device::WaitIdle() const {
        vkDeviceWaitIdle(m_Device);
    }
}
