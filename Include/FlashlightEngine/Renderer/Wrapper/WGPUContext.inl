// Copyright (C) 2025 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

namespace FlashlightEngine::FlWGPU {
    inline WGPUInstance WGPUContext::GetInstance() const {
        return m_Instance;
    }

    inline WGPUSurface WGPUContext::GetSurface() const {
        return m_Surface;
    }

    inline WGPUAdapter WGPUContext::GetAdapter() const {
        return m_Adapter;
    }

    inline WGPUDevice WGPUContext::GetDevice() const {
        return m_Device;
    }
}
