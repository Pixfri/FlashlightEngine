// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#include <FlashlightEngine/Renderer/Renderer.hpp>

namespace FlashlightEngine {
    Renderer::Renderer(const std::shared_ptr<Window>& window, const RendererValidationLevel validationLevel)
        : m_Window(window) {
        window->SetTitle(window->GetTitle() + " <Vulkan 1.3>");

        m_Instance = std::make_shared<Instance>(validationLevel);

        m_Surface = std::make_shared<Surface>(m_Instance, *window);

        m_Device = std::make_shared<Device>(*m_Instance, m_Surface, validationLevel != RendererValidationLevel::None);

        Swapchain::SwapchainInfo swapchainInfo = {
            .Extent = window->GetExtent(),
            .EnableVSync = window->IsVSync(),
            .Surface = m_Surface->GetSurface(),
            .OldSwapchain = nullptr
        };

        m_Swapchain = std::make_shared<Swapchain>(m_Device, swapchainInfo);
    }

    Renderer::~Renderer() = default;

    void Renderer::UpdateSwapchain() {
        m_Window->UpdateFullscreenMode();

        if (m_Window->ShouldInvalidateSwapchain()) {
            Swapchain::SwapchainInfo swapchainInfo = {
                .Extent = m_Window->GetExtent(),
                .EnableVSync = m_Window->IsVSync(),
                .Surface = m_Surface->GetSurface(),
                .OldSwapchain = m_Swapchain
            };

            m_Swapchain = std::make_shared<Swapchain>(m_Device, swapchainInfo);

            m_Window->SwapchainInvalidated();
        }
    }
}
