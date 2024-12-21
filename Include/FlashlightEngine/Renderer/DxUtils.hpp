// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#ifndef FL_RENDERER_DXUTILS_HPP
#define FL_RENDERER_DXUTILS_HPP

#include <FlashlightEngine/Types.hpp>

#include <wrl/client.h>

#include <d3d11.h>
#include <dxgi1_3.h>
#include <DirectXMath.h>

namespace FlashlightEngine {
    template <typename T>
    using ComPtr = Microsoft::WRL::ComPtr<T>;

    inline const char* HResultToString(HRESULT hr);
};

#include <FlashlightEngine/Renderer/DxUtils.inl>

#endif // FL_RENDERER_DXUTILS_HPP
