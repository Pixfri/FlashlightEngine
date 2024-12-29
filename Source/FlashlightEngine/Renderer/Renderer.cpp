// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#include <FlashlightEngine/Renderer/Renderer.hpp>

namespace FlashlightEngine {
    Renderer::Renderer(const std::shared_ptr<Window>& window, const RendererValidationLevel validationLevel)
        : m_Window(window) {
        m_Instance = std::make_unique<Instance>(validationLevel);
    }

    Renderer::~Renderer() {
    }
}
