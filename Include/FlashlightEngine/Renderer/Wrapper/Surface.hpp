// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#ifndef FL_RENDERER_WRAPPER_SURFACE_HPP
#define FL_RENDERER_WRAPPER_SURFACE_HPP

#include <FlashlightEngine/Types.hpp>

#include <FlashlightEngine/Renderer/Wrapper/Instance.hpp>

#include <FlashlightEngine/Core/Window.hpp>

#include <GLFW/glfw3.h>

namespace FlashlightEngine {
    class Surface {
    public:
        Surface(const std::shared_ptr<Instance>& instance, const Window& window);
        ~Surface();

        Surface(const Surface&) = delete;
        Surface(Surface&& other) noexcept;

        [[nodiscard]] inline VkSurfaceKHR GetSurface() const;
        [[nodiscard]] inline bool IsValid() const;

        Surface& operator=(const Surface&) = delete;
        Surface& operator=(Surface&& other) noexcept;
    
    private:
        VkSurfaceKHR m_Surface{VK_NULL_HANDLE};

        std::shared_ptr<Instance> m_Instance{nullptr};
    };
}

#include <FlashlightEngine/Renderer/Wrapper/Surface.inl>

#endif // FL_RENDERER_WRAPPER_SURFACE_HPP
