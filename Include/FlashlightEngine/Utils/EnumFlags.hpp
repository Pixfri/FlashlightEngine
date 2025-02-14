// Copyright (C) 2025 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#ifndef FL_UTILS_ENUMFLAGS_HPP
#define FL_UTILS_ENUMFLAGS_HPP

#include <type_traits>

/**
 * Creates bitwise operators (NOT ~, AND &, OR |, XOR ^, and their assignment counterparts) for the given Enum type.
 * This allows an enum to be used as a flag.
 * @param Enum Enum type to create the operators for.
 */
#define FL_MAKE_ENUM_FLAG(Enum)                                                  \
  constexpr Enum operator~(Enum value) noexcept {                                \
    return static_cast<Enum>(~static_cast<std::underlying_type_t<Enum>>(value)); \
  }                                                                              \
                                                                                 \
  constexpr Enum operator&(Enum lhs, Enum rhs) noexcept {                        \
    using EnumT = std::underlying_type_t<Enum>;                                  \
    return static_cast<Enum>(static_cast<EnumT>(lhs) & static_cast<EnumT>(rhs)); \
  }                                                                              \
                                                                                 \
  constexpr Enum& operator&=(Enum& lhs, Enum rhs) noexcept {                     \
    lhs = lhs & rhs;                                                             \
    return lhs;                                                                  \
  }                                                                              \
                                                                                 \
  constexpr Enum operator|(Enum lhs, Enum rhs) noexcept {                        \
    using EnumT = std::underlying_type_t<Enum>;                                  \
    return static_cast<Enum>(static_cast<EnumT>(lhs) | static_cast<EnumT>(rhs)); \
  }                                                                              \
                                                                                 \
  constexpr Enum& operator|=(Enum& lhs, Enum rhs) noexcept {                     \
    lhs = lhs | rhs;                                                             \
    return lhs;                                                                  \
  }                                                                              \
                                                                                 \
  constexpr Enum operator^(Enum lhs, Enum rhs) noexcept {                        \
    using EnumT = std::underlying_type_t<Enum>;                                  \
    return static_cast<Enum>(static_cast<EnumT>(lhs) ^ static_cast<EnumT>(rhs)); \
  }                                                                              \
                                                                                 \
  constexpr Enum& operator^=(Enum& lhs, Enum rhs) noexcept {                     \
    lhs = lhs ^ rhs;                                                             \
    return lhs;                                                                  \
  }

namespace Fl {
    template <typename T>
    constexpr T Bit(T x) {
        return 1 << x;
    }
}

#endif // FL_UTILS_ENUMFLAGS_HPP
