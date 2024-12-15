// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#ifndef FL_VULKANRHI_SEMAPHORE_HPP
#define FL_VULKANRHI_SEMAPHORE_HPP

#include <FlashlightEngine/Types.hpp>

#include <FlashlightEngine/VulkanRHI/Device.hpp>

#include <memory>

namespace FlashlightEngine::Vk {
    class Semaphore {
    public:
        explicit Semaphore(const std::shared_ptr<Device>& device, VkSemaphoreCreateFlags flags = 0);
        ~Semaphore();

        Semaphore(const Semaphore&) = delete;
        Semaphore(Semaphore&&) = delete;

        [[nodiscard]] inline VkSemaphore GetHandle() const;

        Semaphore& operator=(const Semaphore&) = delete;
        Semaphore& operator=(Semaphore&&) = delete;
    
    private:
        VkSemaphore m_Semaphore{VK_NULL_HANDLE};

        std::shared_ptr<Device> m_Device;
    };
}

#include <FlashlightEngine/VulkanRHI/Semaphore.inl>

#endif // FL_VULKANRHI_SEMAPHORE_HPP
