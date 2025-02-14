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

#   if defined(__MINGW32__)
#       define FL_COMPILER_MINGW
#   endif
#elif defined(__clang__)
#   define FL_COMPILER_CLANG

#   if defined(__MINGW32__)
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

#if defined(FL_COMPILER_MSVC)
#   define DO_PRAGMA(Arg) __pragma(Arg)

#   define PUSH_WARNINGS_STATE DO_PRAGMA(warning(push))
#   define POP_WARNINGS_STATE DO_PRAGMA(warning(pop))

#   define DISABLE_WARNING(Warning) DO_PRAGMA(warning(disable : Warning))

#   define DISABLE_WARNING_MSVC(Warning) DISABLE_WARNING(Warning)
#   define DISABLE_WARNING_GCC(Warning)
#   define DISABLE_WARNING_CLANG(Warning)
#elif defined(FL_COMPILER_GCC) || defined(FL_COMPILER_CLANG)
#   define DO_PRAGMA(Arg) _Pragma(#Arg)

#   define PUSH_WARNINGS_STATE DO_PRAGMA(GCC diagnostic push)
#   define POP_WARNINGS_STATE DO_PRAGMA(GCC diagnostic pop)

#   define DISABLE_WARNING(Warning) DO_PRAGMA(GCC diagnostic ignored #Warning)

#   define DISABLE_WARNING_MSVC(Warning)

#   if defined(FL_COMPILER_CLANG)
#       define DISABLE_WARNING_GCC(Warning)
#       define DISABLE_WARNING_CLANG(Warning) DISABLE_WARNING(Warning)
#   else
#       define DISABLE_WARNING_GCC(Warning) DISABLE_WARNING(Warning)
#       define DISABLE_WARNING_CLANG(Warning)
#   endif
#else
#   define DO_PRAGMA(Arg)
#   define PUSH_WARNINGS_STATE
#   define POP_WARNINGS_STATE
#   define DISABLE_WARNING(Warning)
#   define DISABLE_WARNING_MSVC(Warning)
#   define DISABLE_WARNING_GCC(Warning)
#   define DISABLE_WARNING_CLANG(Warning)
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
