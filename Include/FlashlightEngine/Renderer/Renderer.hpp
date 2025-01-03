// Copyright (C) 2025 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#ifndef FL_RENDERER_RENDERER_HPP
#define FL_RENDERER_RENDERER_HPP

#include <FlashlightEngine/Types.hpp>

#include <FlashlightEngine/Renderer/Wrapper/WGPUContext.hpp>

namespace FlashlightEngine {
    class Renderer {
    public:
        Renderer(const Window& window, bool useHighPerfGPU);
        ~Renderer();

        Renderer(const Renderer&) = delete;
        Renderer(Renderer&&) noexcept = default;

        Renderer& operator=(const Renderer&) = delete;
        Renderer& operator=(Renderer&&) noexcept = default;
    
    private:
        std::shared_ptr<FlWGPU::WGPUContext> m_Context{nullptr};
    };
}

#include <FlashlightEngine/Renderer/Renderer.inl>

#endif // FL_RENDERER_RENDERER_HPP
