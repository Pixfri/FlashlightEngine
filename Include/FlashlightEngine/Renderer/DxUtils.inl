// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#include <comdef.h>

namespace FlashlightEngine {
    inline const char* HResultToString(const HRESULT hr) {
        const _com_error error(hr);

        return error.ErrorMessage();
    }
}
