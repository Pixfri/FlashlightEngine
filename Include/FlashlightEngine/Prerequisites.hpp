// Copyright (C) 2025 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#ifndef FL_PREREQUISITES_HPP
#define FL_PREREQUISITES_HPP

#include <cstdint>

#if defined(_MSC_VER)
#   define FL_COMPILER_MSVC
#elif defined(__GNUC__)
#   define FL_COMPILER_GCC

#   ifdef (__MINGW32__)
#       define FL_COMPILER_MINGW
#   endif
#elif defined(__clang__)
#   define FL_COMPILER_CLANG

#   ifdef (__MINGW32__)
#       define FL_COMPILER_MINGW
#   endif
#else
#   define FL_COMPILER_UNKNOWN
#   error Could not determine the used compiler.
#endif

#if defined(_WIN32)
#   define FL_PLATFORM_WINDOWS
#elif defined(__linux__)
#   define FL_PLATFORM_LINUX
#   define FL_PLATFORM_POSIX
#else
#   define FL_PLATFORM_UNKNOWN
#   error Could not determine the target platform.
#endif

namespace Fl {
    using I8 = int8_t;
    using I16 = int16_t;
    using I32 = int32_t;
    using I64 = int64_t;

    using U8 = uint8_t;
    using U16 = uint16_t;
    using U32 = uint32_t;
    using U64 = uint64_t;

    using F32 = float;
    using F64 = double;
}

#endif // FL_PREREQUISITES_HPP
