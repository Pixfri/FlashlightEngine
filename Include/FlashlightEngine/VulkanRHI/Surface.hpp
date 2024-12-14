// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#ifndef FL_VULKANRHI_SURFACE_HPP
#define FL_VULKANRHI_SURFACE_HPP

#include <FlashlightEngine/Types.hpp>

#include <FlashlightEngine/VulkanRHI/Instance.hpp>

#include <FlashlightEngine/Core/Window.hpp>

#include <volk.h>

#include <memory>

namespace FlashlightEngine::Vk {
    class Surface {
    public:
        Surface(const std::shared_ptr<Instance>& instance, const std::shared_ptr<Window>& window);
        ~Surface();

        Surface(const Surface&) = delete;
        Surface(Surface&&) = delete;

        [[nodiscard]] inline VkSurfaceKHR GetHandle() const;

        Surface& operator=(const Surface&) = delete;
        Surface& operator=(Surface&&) = delete;
    
    private:
        VkSurfaceKHR m_Surface{VK_NULL_HANDLE};

        std::shared_ptr<Instance> m_Instance;
        std::shared_ptr<Window> m_Window;
    };
}

#include <FlashlightEngine/VulkanRHI/Surface.inl>

#endif // FL_VULKANRHI_SURFACE_HPP
