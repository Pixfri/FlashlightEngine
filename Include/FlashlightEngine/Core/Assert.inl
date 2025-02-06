// Copyright (C) 2025 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#include <cassert>

namespace Fl {
    template <typename T>
    void FlAssert(T expr, const std::string& error, std::source_location& location) {
        if (!expr) {;
            spdlog::critical("{} (In {} at ({}:{}) in {})", error, location.file_name(), location.line(), location.column(),
                             location.function_name());

            assert(expr);
        }
    }
}
