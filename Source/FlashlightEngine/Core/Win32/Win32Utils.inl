// Copyright (C) 2025 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#include <FlashlightEngine/Core/Win32/Win32Utils.hpp>

namespace Fl::PlatformImpl {
    inline WidePathHolder PathToWideTemp(const std::filesystem::path& path) {
        if constexpr (ArePathsWide) {
            return path.native();
        } else {
#if defined(FL_COMPILER_MSVC)
            FL_WARNING_PUSH()
            FL_WARNING_MSVC_DISABLE(4840) //< C4840: Unreachable code
#endif

        return path.wstring();

#if defined(FL_COMPILER_MSVC)
            FL_WARNING_POP()
#endif
        }
    }
}
