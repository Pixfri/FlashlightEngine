// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#ifndef FL_VULKANRHI_RENDERCONTEXT_HPP
#define FL_VULKANRHI_RENDERCONTEXT_HPP

#include <FlashlightEngine/Types.hpp>

#include <FlashlightEngine/VulkanRHI/Device.hpp>

#include <FlashlightEngine/Core/Window.hpp>

#include <memory>

namespace FlashlightEngine::Vk {
    class RenderContext {
    public:
        explicit RenderContext(const std::shared_ptr<Window>& window);
        ~RenderContext();

        RenderContext(const RenderContext&) = delete;
        RenderContext(RenderContext&&) = delete;

        RenderContext& operator=(const RenderContext&) = delete;
        RenderContext& operator=(RenderContext&&) = delete;
    
    private:
        std::shared_ptr<Window> m_Window;

        std::shared_ptr<Instance> m_VulkanInstance;
        std::shared_ptr<Surface> m_VulkanSurface;
        std::shared_ptr<Device> m_VulkanDevice;
    };
}

#include <FlashlightEngine/VulkanRHI/RenderContext.inl>

#endif // FL_VULKANRHI_RENDERCONTEXT_HPP
