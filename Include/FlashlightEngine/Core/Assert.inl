// Copyright (C) 2025 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#include <cassert>

namespace Fl {
    template <typename T, typename... Args>
    void FlAssert(T expr, const spdlog::format_string_t<Args...>& fmt, Args&&... args,
                  const std::source_location& location) {
        if (!expr) {
            spdlog::critical(fmt, std::forward<Args>(args)...);
            spdlog::critical("In {} at ({}:{}) in {}", location.file_name(), location.line(), location.column(),
                             location.function_name());

            assert(expr);
        }
    }
}
