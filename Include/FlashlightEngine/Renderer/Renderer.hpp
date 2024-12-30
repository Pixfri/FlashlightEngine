// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#ifndef FL_RENDERER_RENDERER_HPP
#define FL_RENDERER_RENDERER_HPP

#include <FlashlightEngine/Types.hpp>

#include <FlashlightEngine/Renderer/Enums.hpp>
#include <FlashlightEngine/Renderer/Wrapper/Instance.hpp>
#include <FlashlightEngine/Renderer/Wrapper/Surface.hpp>
#include <FlashlightEngine/Renderer/Wrapper/Device.hpp>
#include <FlashlightEngine/Renderer/Wrapper/Swapchain.hpp>

#include <FlashlightEngine/Core/Window.hpp>

namespace FlashlightEngine {
    class Renderer {
    public:
        Renderer(const std::shared_ptr<Window>& window, RendererValidationLevel validationLevel);
        ~Renderer();

        Renderer(const Renderer&) = delete;
        Renderer(Renderer&&) noexcept = default;

        void UpdateSwapchain();

        Renderer& operator=(const Renderer&) = delete;
        Renderer& operator=(Renderer&&) noexcept = default;

    private:
        std::shared_ptr<Window> m_Window;

        std::shared_ptr<Instance> m_Instance;
        std::shared_ptr<Surface> m_Surface;
        std::shared_ptr<Device> m_Device;
        std::shared_ptr<Swapchain> m_Swapchain;
    };
}

#include <FlashlightEngine/Renderer/Renderer.inl>

#endif // FL_RENDERER_RENDERER_HPP
