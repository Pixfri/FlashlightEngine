// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#include <FlashlightEngine/Renderer/Renderer.hpp>

namespace FlashlightEngine {
    Renderer::Renderer(const std::shared_ptr<Window>& window)
        : m_Window(window) {
        m_Device = std::make_shared<Device>();
        m_Swapchain = std::make_shared<Swapchain>(m_Window, m_Device);
    }

    void Renderer::OnResize(const UInt32 width, const UInt32 height) const {
        m_Swapchain->OnResize(width, height);
    }
}
