// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#ifndef FL_CORE_COREUTILS_HPP
#define FL_CORE_COREUTILS_HPP

#include <source_location>
#include <string>
#include <type_traits>

namespace FlashlightEngine {
    template <typename T>
    constexpr void FlAssert(T expr, const std::string& msg = "No information provided",
                            const std::source_location& location = std::source_location::current());

    template <typename T>
    constexpr T Bit(T x);

    template <typename Enum, std::enable_if_t<std::is_enum_v<Enum>, bool> = true>
    constexpr auto EnumAsInteger(Enum value) -> std::underlying_type_t<Enum>;
}

#include <FlashlightEngine/Core/CoreUtils.inl>

#endif // FL_CORE_COREUTILS_HPP
