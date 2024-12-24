// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#ifndef FL_RENDERER_DXUTILS_HPP
#define FL_RENDERER_DXUTILS_HPP

#include <FlashlightEngine/Types.hpp>

#include <wrl/client.h>

#include <directx/d3d12.h>
#include <directx/d3dx12.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

namespace FlashlightEngine {
    template <typename T>
    using ComPtr = Microsoft::WRL::ComPtr<T>;

    inline std::string HResultToString(HRESULT hr);

    inline void ThrowIfFailed(HRESULT hr);
};

#include <FlashlightEngine/Renderer/DxUtils.inl>

#endif // FL_RENDERER_DXUTILS_HPP
