// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#ifndef FL_VULKANRHI_COMMANDBUFFER_HPP
#define FL_VULKANRHI_COMMANDBUFFER_HPP

#include <FlashlightEngine/Types.hpp>

#include <FlashlightEngine/VulkanRHI/Device.hpp>
#include <FlashlightEngine/VulkanRHI/Enums.hpp>
#include <FlashlightEngine/VulkanRHI/Semaphore.hpp>
#include <FlashlightEngine/VulkanRHI/Fence.hpp>

namespace FlashlightEngine::Vk {
    class CommandPool {
    public:
        CommandPool(const std::shared_ptr<Device>& device, CommandPoolType type);
        ~CommandPool();

        CommandPool(const CommandPool&) = delete;
        CommandPool(CommandPool&&) = delete;

        [[nodiscard]] inline VkCommandPool GetHandle() const;
        [[nodiscard]] inline CommandPoolType GetType() const;

        void Reset() const;

        CommandPool& operator=(const CommandPool&) = delete;
        CommandPool& operator=(CommandPool&&) = delete;

    private:
        VkCommandPool m_CommandPool{VK_NULL_HANDLE};

        CommandPoolType m_Type;

        std::shared_ptr<Device> m_Device;
    };

    class CommandBuffer {
    public:
        explicit CommandBuffer(const std::shared_ptr<Device>& device,
                               const std::shared_ptr<CommandPool>& commandPool,
                               bool allocate = true);
        ~CommandBuffer();

        CommandBuffer(const CommandBuffer&) = delete;
        CommandBuffer(CommandBuffer&&) = delete;

        [[nodiscard]] inline VkCommandBuffer GetHandle() const;

        void Allocate();
        void Free();

        // ====================== Commands ====================== //
        void Begin(VkCommandBufferUsageFlags flags = 0);
        void End();

        void Reset(VkCommandBufferResetFlags flags = 0);

        void Submit(const std::vector<Semaphore>& waitSemaphores,
                    const std::vector<Semaphore>& signalSemaphores,
                    const std::vector<PipelineStages>& waitStages,
                    const Fence& signalFence);

        CommandBuffer& operator=(const CommandBuffer&) = delete;
        CommandBuffer& operator=(CommandBuffer&&) = delete;

    private:
        VkCommandBuffer m_CommandBuffer{VK_NULL_HANDLE};

        CommandBufferState m_State = CommandBufferState::Invalid;

        bool m_IsOneTime = false;

        std::shared_ptr<Device> m_Device;
        std::shared_ptr<CommandPool> m_CommandPool;
    };
}

#include <FlashlightEngine/VulkanRHI/CommandBuffer.inl>

#endif // FL_VULKANRHI_COMMANDBUFFER_HPP
