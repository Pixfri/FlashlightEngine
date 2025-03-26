// Copyright (C) 2025 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#ifndef FL_CORE_ASSERT_HPP
#define FL_CORE_ASSERT_HPP

#include <FlashlightEngine/Prerequisites.hpp>

#include <fmt/core.h>
#include <fmt/color.h>
#include <fmt/format.h>

#if defined(FL_DEBUG)
#   define FlAssert(expr, format, ...)                          \
        do {                                                    \
            if FL_UNLIKELY(!(expr)) {                           \
                fmt::println(format __VA_OPT__(,) __VA_ARGS__); \
                if (Fl::IsDebuggerAttached()) {                 \
                    FlDebugBreak();                             \
                }                                               \
            }                                                   \
        } while (false)
#else
#   define FlAssert(expr, fmt, ...) ((void)0)
#endif

#define FlError(format, ...) {                      \
    fmt::println(format __VA_OPT__(,) __VA_ARGS__); \
    if (Fl::IsDebuggerAttached()) {                 \
        FlDebugBreak();                             \
    }                                               \
}

#endif // FL_CORE_ASSERT_HPP
