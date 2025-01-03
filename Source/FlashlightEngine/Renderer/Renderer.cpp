// Copyright (C) 2025 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#include <FlashlightEngine/Renderer/Renderer.hpp>

namespace FlashlightEngine {
    Renderer::Renderer(const Window& window, const bool useHighPerfGPU) {
        m_Context = std::make_unique<FlWGPU::WGPUContext>(window, useHighPerfGPU);
    }

    Renderer::~Renderer() = default;
}
