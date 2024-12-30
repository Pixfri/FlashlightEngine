// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

namespace FlashlightEngine {
    inline VkPhysicalDevice Device::GetPhysicalDevice() const {
        return m_PhysicalDevice;
    }

    inline VkPhysicalDeviceProperties Device::GetPhysicalDeviceProperties() const {
        VkPhysicalDeviceProperties properties;
        vkGetPhysicalDeviceProperties(m_PhysicalDevice, &properties);

        return properties;
    }

    inline QueueFamilyIndices Device::GetQueueFamilies() const {
        return FindQueueFamilies(m_PhysicalDevice);
    }

    inline bool Device::IsValid() const {
        return m_PhysicalDevice != VK_NULL_HANDLE;
    }
}
