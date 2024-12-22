// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#ifndef FL_RENDERER_VERTEXTYPES_HPP
#define FL_RENDERER_VERTEXTYPES_HPP

#include <FlashlightEngine/Types.hpp>

#include <FlashlightEngine/Renderer/DxUtils.hpp>

namespace FlashlightEngine {
    enum class VertexType {
        PositionColor,
        PositionColorUv,
    };

    using Position = DirectX::XMFLOAT3;
    using Color = DirectX::XMFLOAT3;
    using Uv = DirectX::XMFLOAT2;

    struct VertexPositionColor {
        Position Position;
        Color Color;
    };

    struct VertexPositionColorUv {
        Position Position;
        Color Color;
        Uv Uv;
    };
}

#endif // FL_RENDERER_VERTEXTYPES_HPP
