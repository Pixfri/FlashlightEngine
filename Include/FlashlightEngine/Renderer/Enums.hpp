// Copyright (C) 2025 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#ifndef FL_RENDERER_ENUMS_HPP
#define FL_RENDERER_ENUMS_HPP

#include <FlashlightEngine/Types.hpp>

namespace FlashlightEngine {
    enum class BackendType {
        Default = 0,

        WebGPU,
        D3D11,
        D3D12,
        Metal,
        Vulkan,
        OpenGL,
        OpenGLES
    };
}

#endif // FL_RENDERER_ENUMS_HPP
