// Copyright (C) 2025 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#ifndef FL_UTILITY_CONSTANTEVALUATED_HPP
#define FL_UTILITY_CONSTANTEVALUATED_HPP

#include <FlashlightEngine/Prerequisites.hpp>

#include <type_traits>

#if FL_CHECK_CPP_VER(FL_CPP23)
#   define FL_HAS_CONSTEVAL
#   define FL_CONSTEXPR20 constexpr
#   define FL_IS_CONSTEVAL() consteval
#   define FL_IS_RUNTIME_EVAL() !consteval
#elif FL_CHECK_CPP_VER(FL_CPP20)
#   define FL_HAS_CONSTEVAL
#   define FL_CONSTEXPR20 constexpr
#   define FL_IS_CONSTEVAL() (std::is_constant_evaluated())
#   define FL_IS_RUNTIME_EVAL() (!std::is_constant_evaluated())
#elif FL_CHECK_MSVC_VER(1925) || FL_CHECK_CLANG_VER(900) || FL_CHECK_GCC_VER(900)
// Supported through compiler extension
#   define FL_HAS_CONSTEVAL
#   define FL_CONSTEXPR20 constexpr
#   define FL_IS_CONSTEVAL() (__builtin_is_constant_evaluated())
#   define FL_IS_RUNTIME_EVAL() (!__builtin_is_constant_evaluated())
#else
#   define FL_CONSTEXPR20
#   define FL_IS_CONSTEVAL() (false)
#   define FL_IS_RUNTIME_EVAL() (true)
#endif

#endif // FL_UTILITY_CONSTANTEVALUATED_HPP
