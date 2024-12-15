// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#ifndef FL_VULKANRHI_FENCE_HPP
#define FL_VULKANRHI_FENCE_HPP

#include <FlashlightEngine/Types.hpp>

#include <FlashlightEngine/VulkanRHI/Device.hpp>

namespace FlashlightEngine::Vk {
    class Fence {
    public:
        explicit Fence(const std::shared_ptr<Device>& device, VkFenceCreateFlags flags = 0);
        ~Fence();

        Fence(const Fence&) = delete;
        Fence(Fence&&) = delete;

        [[nodiscard]] inline VkFence GetHandle() const;

        void Wait(UInt64 timeout = std::numeric_limits<UInt64>::max()) const;
        void Reset() const;

        Fence& operator=(const Fence&) = delete;
        Fence& operator=(Fence&&) = delete;
    
    private:
        VkFence m_Fence{VK_NULL_HANDLE};

        std::shared_ptr<Device> m_Device;
    };
}

#include <FlashlightEngine/VulkanRHI/Fence.inl>

#endif // FL_VULKANRHI_FENCE_HPP
