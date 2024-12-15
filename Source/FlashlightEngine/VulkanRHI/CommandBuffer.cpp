// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#include <FlashlightEngine/VulkanRHI/CommandBuffer.hpp>

#include <FlashlightEngine/Core/Logger.hpp>

namespace FlashlightEngine::Vk {
    CommandPool::CommandPool(const std::shared_ptr<Device>& device, const CommandPoolType type)
        : m_Type(type) {
        UInt32 queueFamily = 0;

        const QueueFamilyIndices queueFamilies = device->GetQueueIndices();

        switch (type) {
        case CommandPoolType::Graphics:
            {
                if (!queueFamilies.GraphicsFamilyFound) {
                    Log::Warn("No graphics queue family is available for the command pool.");
                }

                queueFamily = queueFamilies.GraphicsFamily;
            }
            break;
        case CommandPoolType::Compute:
            {
                if (!queueFamilies.ComputeFamilyFound) {
                    Log::Warn("No compute queue family is available for the command pool.");
                }

                queueFamily = queueFamilies.ComputeFamily;
            }
            break;
        case CommandPoolType::Transfer:
            {
                if (!queueFamilies.TransferFamilyFound) {
                    Log::Warn("No transfer queue family is available for the command pool.");
                }

                queueFamily = queueFamilies.TransferFamily;
            }
            break;
        }

        VkCommandPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.queueFamilyIndex = queueFamily;
        // Provide the ability to reset command buffers.
        poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

        if (vkCreateCommandPool(device->GetDevice(), &poolInfo, nullptr, &m_CommandPool) != VK_SUCCESS) {
            Log::Error("[Vulkan] Failed to create command pool!");
            throw std::runtime_error("Failed to create command pool!");
        }
    }

    CommandPool::~CommandPool() {
        if (m_CommandPool != VK_NULL_HANDLE) {
            vkDestroyCommandPool(m_Device->GetDevice(), m_CommandPool, nullptr);
        }
    }

    void CommandPool::Reset() const {
        vkResetCommandPool(m_Device->GetDevice(), m_CommandPool, 0);
    }

    CommandBuffer::CommandBuffer(const std::shared_ptr<Device>& device,
                                 const std::shared_ptr<CommandPool>& commandPool,
                                 const bool allocate)
        : m_Device(device), m_CommandPool(commandPool) {
        if (allocate) {
            Allocate();
        }
    }

    CommandBuffer::~CommandBuffer() {
        if (m_CommandBuffer != VK_NULL_HANDLE) {
            vkFreeCommandBuffers(m_Device->GetDevice(), m_CommandPool->GetHandle(), 1, &m_CommandBuffer);
        }
    }

    void CommandBuffer::Allocate() {
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = m_CommandPool->GetHandle();
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = 1;

        if (vkAllocateCommandBuffers(m_Device->GetDevice(), &allocInfo, &m_CommandBuffer) != VK_SUCCESS) {
            Log::Error("[Vulkan] Failed to allocate command buffer!");
            throw std::runtime_error("Failed to allocate command buffer!");
        }

        m_State = CommandBufferState::Initial;
    }

    void CommandBuffer::Free() {
        vkFreeCommandBuffers(m_Device->GetDevice(), m_CommandPool->GetHandle(), 1, &m_CommandBuffer);

        m_State = CommandBufferState::Invalid;
    }

    void CommandBuffer::Begin(const VkCommandBufferUsageFlags flags) {
        if (m_CommandBuffer == VK_NULL_HANDLE) {
            Log::Warn("[Vulkan] Tried to begin command buffer that is not allocated!");
            return;
        }

        if (m_State != CommandBufferState::Initial) {
            Log::Warn("[Vulkan] Tried to begin command buffer that is already in use or not allocated!");
            return;
        }

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = flags;
        beginInfo.pInheritanceInfo = nullptr;

        if (flags & VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT) {
            m_IsOneTime = true;
        }

        if (vkBeginCommandBuffer(m_CommandBuffer, &beginInfo) != VK_SUCCESS) {
            Log::Error("[Vulkan] Failed to begin command buffer!");
            throw std::runtime_error("Failed to begin command buffer!");
        }

        m_State = CommandBufferState::Recording;
    }

    void CommandBuffer::End() {
        if (m_CommandBuffer == VK_NULL_HANDLE) {
            Log::Warn("[Vulkan] Tried to end command buffer that is not allocated!");
            return;
        }

        if (m_State != CommandBufferState::Recording) {
            Log::Warn("[Vulkan] Tried to end command buffer that is not in use or not allocated!");
            return;
        }

        if (vkEndCommandBuffer(m_CommandBuffer) != VK_SUCCESS) {
            Log::Error("[Vulkan] Failed to record commands in command buffer!");
            throw std::runtime_error("Failed to record commands in command buffer!");
        }

        m_State = CommandBufferState::Executable;
    }

    void CommandBuffer::Reset(const VkCommandBufferResetFlags flags) {
        if (m_CommandBuffer == VK_NULL_HANDLE) {
            Log::Warn("[Vulkan] Tried to reset command buffer that is not allocated!");
            return;
        }

        vkResetCommandBuffer(m_CommandBuffer, flags);

        m_State = CommandBufferState::Initial;
    }

    void CommandBuffer::Submit(const std::vector<Semaphore>& waitSemaphores,
                               const std::vector<Semaphore>& signalSemaphores,
                               const std::vector<PipelineStages>& waitStages,
                               const Fence& signalFence) {
        if (m_CommandBuffer == VK_NULL_HANDLE) {
            Log::Warn("[Vulkan] Tried to submit command buffer that is not allocated!");
            return;
        }

        if (m_State != CommandBufferState::Executable) {
            Log::Warn("[Vulkan] Tried to submit command buffer that is not in use or not allocated!");
            return;
        }

        std::vector<VkSemaphore> nativeWaitSemaphores(waitSemaphores.size());
        std::vector<VkSemaphore> nativeSignalSemaphores(signalSemaphores.size());

        for (UInt32 i = 0; i < waitSemaphores.size(); i++) {
            nativeWaitSemaphores[i] = waitSemaphores[i].GetHandle();
            nativeSignalSemaphores[i] = signalSemaphores[i].GetHandle();
        }

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.waitSemaphoreCount = static_cast<UInt32>(waitSemaphores.size());
        submitInfo.pWaitSemaphores = nativeWaitSemaphores.data();
        submitInfo.pWaitDstStageMask = waitStages.data();
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &m_CommandBuffer;
        submitInfo.signalSemaphoreCount = static_cast<UInt32>(signalSemaphores.size());
        submitInfo.pSignalSemaphores = nativeSignalSemaphores.data();

        VkQueue queue{VK_NULL_HANDLE};

        switch (m_CommandPool->GetType()) {
            case CommandPoolType::Graphics:
                queue = m_Device->GetGraphicsQueue();
                break;
            case CommandPoolType::Compute:
                queue = m_Device->GetComputeQueue();
                break;
            case CommandPoolType::Transfer:
                queue = m_Device->GetTransferQueue();
                break;
        }

        vkQueueSubmit(queue, 1, &submitInfo, signalFence.GetHandle());

        m_State = m_IsOneTime ? CommandBufferState::Invalid : CommandBufferState::Executable;
    }
}
