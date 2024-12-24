// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#include <FlashlightEngine/Core/CoreUtils.hpp>

#include <comdef.h>

namespace FlashlightEngine {
    inline std::string HResultToString(const HRESULT hr) {
        const _com_error error(hr);

        return {error.ErrorMessage()};
    }

    inline void ThrowIfFailed(const HRESULT hr) {
        FlAssert(SUCCEEDED(hr), "[DirectX] Call failed. Detail: " + HResultToString(hr));
    }
}
