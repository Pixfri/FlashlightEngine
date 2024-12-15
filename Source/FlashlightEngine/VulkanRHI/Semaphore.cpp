// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#include <FlashlightEngine/VulkanRHI/Semaphore.hpp>

#include <FlashlightEngine/Core/Logger.hpp>

namespace FlashlightEngine::Vk {
    Semaphore::Semaphore(const std::shared_ptr<Device>& device, const VkSemaphoreCreateFlags flags)
        : m_Device(device) {
        VkSemaphoreCreateInfo semaphoreCreateInfo = {};
        semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
        semaphoreCreateInfo.pNext = nullptr;

        semaphoreCreateInfo.flags = flags;

        if (vkCreateSemaphore(device->GetDevice(), &semaphoreCreateInfo, nullptr, &m_Semaphore) != VK_SUCCESS) {
            Log::Error("[Vulkan] Failed to create semaphore!");
            throw std::runtime_error("Failed to create semaphore!");
        }
    }

    Semaphore::~Semaphore() {
        if (m_Semaphore != VK_NULL_HANDLE) {
            vkDestroySemaphore(m_Device->GetDevice(), m_Semaphore, nullptr);
        }
    }
}
