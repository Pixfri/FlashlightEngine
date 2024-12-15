// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#include <FlashlightEngine/VulkanRHI/Fence.hpp>

#include <FlashlightEngine/Core/Logger.hpp>

namespace FlashlightEngine::Vk {
    Fence::Fence(const std::shared_ptr<Device>& device, const VkFenceCreateFlags flags)
        : m_Device(device) {
        VkFenceCreateInfo fenceCreateInfo = {};
        fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceCreateInfo.pNext = nullptr;

        fenceCreateInfo.flags = flags;

        if (vkCreateFence(m_Device->GetDevice(), &fenceCreateInfo, nullptr, &m_Fence) != VK_SUCCESS) {
            Log::Error("[Vulkan] Failed to create fence!");
            throw std::runtime_error("Failed to create fence!");
        }
    }

    Fence::~Fence() {
        if (m_Fence != VK_NULL_HANDLE) {
            vkDestroyFence(m_Device->GetDevice(), m_Fence, nullptr);
        }
    }

    void Fence::Wait(const UInt64 timeout) const {
        vkWaitForFences(m_Device->GetDevice(), 1, &m_Fence, VK_TRUE, timeout);
    }

    void Fence::Reset() const {
        if (vkResetFences(m_Device->GetDevice(), 1, &m_Fence) != VK_SUCCESS) {
            Log::Error("[Vulkan] Failed to reset fence!");
        }
    }
}
