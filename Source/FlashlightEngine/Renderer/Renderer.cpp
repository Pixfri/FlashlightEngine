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
    }

    Renderer::~Renderer() = default;
}
