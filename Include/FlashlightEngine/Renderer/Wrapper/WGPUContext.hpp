// Copyright (C) 2025 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#ifndef FL_RENDERER_WRAPPER_WGPUCONTEXT_HPP
#define FL_RENDERER_WRAPPER_WGPUCONTEXT_HPP

#include <FlashlightEngine/Types.hpp>

#include <FlashlightEngine/Core/Window.hpp>

#include <webgpu/webgpu.h>

namespace FlashlightEngine::FlWGPU {
    class WGPUContext {
    public:
        WGPUContext(const Window& window, bool useHighPerfGPU);
        ~WGPUContext();

        WGPUContext(const WGPUContext&) = delete;
        WGPUContext(WGPUContext&& other) noexcept;

        [[nodiscard]] inline WGPUInstance GetInstance() const;
        [[nodiscard]] inline WGPUSurface GetSurface() const;
        [[nodiscard]] inline WGPUAdapter GetAdapter() const;
        [[nodiscard]] inline WGPUDevice GetDevice() const;

        WGPUContext& operator=(const WGPUContext&) = delete;
        WGPUContext& operator=(WGPUContext&& other) noexcept;

    private:
        WGPUInstance m_Instance{nullptr};
        WGPUSurface m_Surface{nullptr};
        WGPUAdapter m_Adapter{nullptr};
        WGPUDevice m_Device{nullptr};

        void CreateInstance();
        void CreateSurface(const Window& window);
        void RequestAdapter(bool useHighPerfGPU);
        void RequestDevice();

        void PrintAdapterProperties() const;
    };
}

#include <FlashlightEngine/Renderer/Wrapper/WGPUContext.inl>

#endif // FL_RENDERER_WRAPPER_WGPUCONTEXT_HPP
