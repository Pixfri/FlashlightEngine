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
        PositionColor
    };

    using Position = DirectX::XMFLOAT3;
    using Color = DirectX::XMFLOAT3;

    struct VertexPositionColor {
        Position Pos;
        Color Col;
    };
}

#endif // FL_RENDERER_VERTEXTYPES_HPP
