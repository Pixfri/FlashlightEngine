// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#ifndef FL_RENDERER_RENDERER_HPP
#define FL_RENDERER_RENDERER_HPP

#include <FlashlightEngine/Types.hpp>

#include <FlashlightEngine/Core/Window.hpp>

#include <FlashlightEngine/Renderer/Device.hpp>
#include <FlashlightEngine/Renderer/Swapchain.hpp>

namespace FlashlightEngine {
    class Renderer {
    public:
        explicit Renderer(const std::shared_ptr<Window>& window);
        ~Renderer() = default;

        Renderer(const Renderer&) = delete;
        Renderer(Renderer&&) noexcept = default;

        void OnResize(UInt32 width, UInt32 height) const;

        Renderer& operator=(const Renderer&) = delete;
        Renderer& operator=(Renderer&&) noexcept = default;
    
    private:
        std::shared_ptr<Window> m_Window;

        std::shared_ptr<Device> m_Device;
        std::shared_ptr<Swapchain> m_Swapchain;
    };
}

#include <FlashlightEngine/Renderer/Renderer.inl>

#endif // FL_RENDERER_RENDERER_HPP
