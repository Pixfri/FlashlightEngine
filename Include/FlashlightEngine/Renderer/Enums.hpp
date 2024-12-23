// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#ifndef FL_RENDERER_ENUMS_HPP
#define FL_RENDERER_ENUMS_HPP

#include <FlashlightEngine/Types.hpp>

namespace FlashlightEngine {
    enum class PrimitiveTopology {
        Undefined = 0,
        PointList = 1,
        LineList = 2,
        LineStrip = 3,
        TriangleList = 4,
        TriangleStrip = 5,
        LineListAdj = 10,
        LineStripAdj = 11,
        TriangleListAdj = 12,
        TriangleStripAdj = 13
    };

    enum class PipelineBindPoint {
        VertexShader,
        PixelShader,

        // Unsupported for now
        ComputeShader,
        GeometryShader,
        HullShader,
        DomainShader
    };

    enum class IndexType {
        UInt16,
        UInt32
    };
}

#endif // FL_RENDERER_ENUMS_HPP
