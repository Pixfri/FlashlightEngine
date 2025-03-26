// Copyright (C) 2025 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#include <FlashlightEngine/Core/Win32/Win32Utils.hpp>

namespace Fl::PlatformImpl {
    inline WidePathHolder PathToWideTemp(const std::filesystem::path& path) {
        if constexpr (ArePathsWide) {
            return path.native();
        }

        return path.wstring();
    }
}
