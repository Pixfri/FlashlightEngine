// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#include <spdlog/spdlog.h>

namespace FlashlightEngine {
#if defined(FL_DEBUG) || defined(FL_FORCE_ASSERTS)
    template <typename T>
    constexpr void FlAssert(T expr, const std::string& msg, const std::source_location& location) {
        if (!expr) {
            spdlog::error("{0}({1}:{2}) in function {3}. Details: {4}",
                          location.file_name(),
                          location.line(),
                          location.column(),
                          location.function_name(),
                          msg);
        }
        assert(expr);
    }
#else

    template <typename T>
    constexpr void FlAssert([[maybe_unused]] T expr, [[maybe_unused]] const std::string& msg, [[maybe_unused]] const std::source_location& location) {}
#endif

    template <typename T>
    constexpr T Bit(const T x) {
        return 1 << x;
    }

    template <typename Enum, std::enable_if_t<std::is_enum_v<Enum>, bool>>
    constexpr auto EnumAsInteger(Enum value) -> std::underlying_type_t<Enum> {
        return static_cast<std::underlying_type_t<Enum>>(value);
    }
}
