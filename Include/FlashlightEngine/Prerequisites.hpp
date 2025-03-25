// Copyright (C) 2025 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#ifndef FL_PREREQUISITES_HPP
#define FL_PREREQUISITES_HPP


// |------------------------------|
// | Compiler & OS identification |
// |------------------------------|
#if defined(__clang__)
#   define FL_COMPILER_CLANG
#   define FL_COMPILER_CLANG_VER (__clang_major__ * 100 + __clang_minor__)
#   define FL_DEPRECATED(text) __attribute__((__deprecated__(text)))
#   define FL_PRETTY_FUNCTION __PRETTY_FUNCTION__

#   define FL_CHECK_CLANG_VER(ver) (FL_COMPILER_CLANG_VER >= ver)

#   define FL_PRAGMA(x) _Pragma(#x)

#   define FL_WARNING_CLANG_DISABLE(warn) FL_PRAGMA(clang diagnostic ignored warn)
#   define FL_WARNING_CLANG_GCC_DISABLE(warn) FL_PRAGMA(clang diagnostic ignored warn)
#   define FL_WARNING_POP() FL_PRAGMA(clang diagnostic pop)
#   define FL_WARNING_PUSH() FL_PRAGMA(clang diagnostic push)

#   ifdef __MINGW32__
#       define FL_COMPILER_MINGW
#       ifdef __MINGW64_VERSION_MAJOR
#           define FL_COMPILER_MINGW64
#       endif
#   endif
#elif defined(__GNUC__) || defined(__MINGW32__)
#   define FL_COMPILER_GCC
#   define FL_COMPILER_GCC_VER (__GNUC__ * 100 + __GNUC_MINOR__)
#   define FL_DEPRECATED(text) __attribute__((__deprecated__(text)))
#   define FL_PRETTY_FUNCTION __PRETTY_FUNCTION__

#   define FL_CHECK_GCC_VER(ver) (FL_COMPILER_GCC_VER >= ver)

#   define FL_PRAGMA(x) _Pragma(#x)

#   define FL_WARNING_CLANG_GCC_DISABLE(warn) FL_PRAGMA(GCC diagnostic ignored warn)
#   define FL_WARNING_GCC_DISABLE(warn) FL_PRAGMA(GCC diagnostic ignored warn)
#   define FL_WARNING_POP() FL_PRAGMA(GCC diagnostic pop)
#   define FL_WARNING_PUSH() FL_PRAGMA(GCC diagnostic push)

#   ifdef __MINGW32__
#       define FL_COMPILER_MINGW
#       ifdef __MINGW64_VERSION_MAJOR
#           define FL_COMPILER_MINGW64
#       endif
#   endif
#elif defined(__INTEL_COMPILER) || defined(__ICL)
#   define FL_COMPILER_ICC
#   define FL_COMPILER_ICC_VER __INTEL_COMPILER
#   define FL_DEPRECATED(text) [[deprecated(text)]]
#   define FL_PRETTY_FUNCTION __FUNCTION__

#   define FL_CHECK_ICC_VER(ver) (FL_COMPILER_ICC_VER >= ver)

#   define FL_PRAGMA(x) _Pragma(#x)

#   define FL_WARNING_ICC_DISABLE(...) FL_PRAGMA(warning(disable: __VA_ARGS__))
#   define FL_WARNING_POP() FL_PRAGMA(warning(pop))
#   define FL_WARNING_PUSH() FL_PRAGMA(warning(push))
#elif defined(_MSC_VER)
#   define FL_COMPILER_MSVC
#   define FL_COMPILER_MSVC_VER _MSC_VER
#   define FL_DEPRECATED(text) __declspec(deprecated(text))
#   define FL_PRETTY_FUNCTION __FUNCSIG__

#   define FL_CHECK_MSVC_VER(ver) (FL_COMPILER_MSVC_VER >= ver)

#   define FL_PRAGMA(x) __pragma(#x)

#   define FL_WARNING_MSVC_DISABLE(...) FL_PRAGMA(warning(disable: __VA_ARGS__))
#   define FL_WARNING_POP() FL_PRAGMA(warning(pop))
#   define FL_WARNING_PUSH() FL_PRAGMA(warning(push))

// __cplusplus isn't defined on MSVC without the /Zc:__cplusplus flag
#   define FL_CPP_VER _MSVC_LANG
#else
#   define FL_COMPILER_UNKNOWN
#   pragma message This compiler is not fully supported and will probably cause problems when compiling
#endif

// Detect MinGW thread model
#ifdef FL_COMPILER_MINGW
#   if defined(__USING_MCFGTHREAD__)
#       define FL_COMPILER_MINGW_THREADS_MCF
#   elif defined(_REENTRANT)
#       define FL_COMPILER_MINGW_THREADS_POSIX
#   else
#       define FL_COMPILER_MINGW_THREADS_WIN32
#   endif
#endif

#ifndef FL_CHECK_CLANG_VER
#   define FL_CHECK_CLANG_VER(ver) 0
#endif

#ifndef FL_CHECK_GCC_VER
#   define FL_CHECK_GCC_VER(ver) 0
#endif

#ifndef FL_CHECK_ICC_VER
#   define FL_CHECK_ICC_VER(ver) 0
#endif

#ifndef FL_CHECK_MSVC_VER
#   define FL_CHECK_MSVC_VER(ver) 0
#endif

#ifndef FL_CPP_VER
#   define FL_CPP_VER __cplusplus
#endif

#ifndef FL_DEPRECATED
#   define FL_DEPRECATED(text) [[deprecated(text)]]
#endif

#ifndef FL_PRAGMA
#define FL_PRAGMA(x) _Pragma(#x)
#endif

#ifndef FL_PRETTY_FUNCTION
#define FL_PRETTY_FUNCTION __func__
#endif

#ifndef FL_WARNING_CLANG_DISABLE
#   define FL_WARNING_CLANG_DISABLE(warn)
#endif

#ifndef FL_WARNING_CLANG_GCC_DISABLE
#   define FL_WARNING_CLANG_GCC_DISABLE(warn)
#endif

#ifndef FL_WARNING_GCC_DISABLE
#   define FL_WARNING_GCC_DISABLE(warn)
#endif

#ifndef FL_WARNING_ICC_DISABLE
#   define FL_WARNING_ICC_DISABLE(warn)
#endif

#ifndef FL_WARNING_MSVC_DISABLE
#   define FL_WARNING_MSVC_DISABLE(...)
#endif

#ifndef FL_WARNING_POP
#   define FL_WARNING_POP()
#endif

#ifndef FL_WARNING_PUSH
#   define FL_WARNING_PUSH()
#endif

// |----------------------------|
// | C++ version identification |
// |----------------------------|

#define FL_CPP98 199711L
#define FL_CPP11 201103L
#define FL_CPP14 201402L
#define FL_CPP17 201703L
#define FL_CPP20 202002L
#define FL_CPP23 202302L

#define FL_CHECK_CPP_VER(ver) (FL_CPP_VER >= ver)

#if !FL_CHECK_CPP_VER(FL_CPP20)
#    error Flashlight Engine requires C++20 or higher
#endif

// |-------------------------|
// | Platform identification |
// |-------------------------|

#if defined(_WIN32)
#   define FL_PLATFORM_DESKTOP
#   define FL_PLATFORM_WINDOWS

#   define FL_EXPORT __declspec(dllexport)
#   define FL_IMPORT __declspec(dllimport)

// Some defines to strip down Windows.h
#   if defined(FL_BUILD)
#       ifndef WIN32_LEAN_AND_MEAN
#           define WIN32_LEAN_AND_MEAN
#       endif

#       ifndef NOMINMAX
#           define NOMINMAX
#       endif

#       if FL_WINDOWS_NT6
#           define FL_WINNT 0x0600
#       else
#           define FL_WINNT 0x0501
#       endif

// Keep the actual define if existing and greater than our requirement
#       if defined(_WIN32_WINNT)
#           if _WIN32_WINNT < FL_WINNT
#               undef _WIN32_WINNT
#               define WIN32_WINNT FL_WINNT
#           endif
#       else
#           define _WIN32_WINNT FL_WINNT
#       endif
#   endif
#elif defined(__ANDROID__)
// android/ndk-version.h was added with NDK 16 so we should be safe, but an error is better than nothing
#  if !__has_include(<android/ndk-version.h>)
#       error Flashlight Engine requires a more up to date NDK version, please update
#   endif

#   include <android/ndk-version.h>

#   define FL_PLATFORM_MOBILE
#   define FL_PLATFORM_ANDROID
#   define FL_PLATFORM_POSIX

#   define FL_PLATFORM_ANDROID_NDK_VER __NDK_MAJOR__
#   define FL_CHECK_NDK_VER(ver) (FL_PLATFORM_ANDROID_NDK_VER >= ver)

#   define FL_EXPORT __attribute__((visibility("default")))
#   define FL_IMPORT __attribute__((visibility("default")))
#elif defined(__linux__)
#   define FL_PLATFORM_DESKTOP
#   define FL_PLATFORM_LINUX
#   define FL_PLATFORM_POSIX

#   define FL_EXPORT __attribute__((visibility("default")))
#   define FL_IMPORT __attribute__((visibility("default")))
#elif defined(__FreeBSD__)
#   define FL_PLATFORM_BSD
#   define FL_PLATFORM_FREEBSD
#   define FL_PLATFORM_POSIX

#   define FL_EXPORT __attribute__((visibility("default")))
#   define FL_IMPORT __attribute__((visibility("default")))
#elif defined(__APPLE__)
#   include <TargetConditionals.h>
#   if TARGET_OS_IPHONE
#       define FL_PLATFORM_MOBILE
#       define FL_PLATFORM_IOS
#   else
#       define FL_PLATFORM_DESKTOP
#       define FL_PLATFORM_MACOS
#   endif
#   define FL_PLATFORM_BSD
#   define FL_PLATFORM_POSIX

#   define FL_EXPORT __attribute__((visibility("default")))
#   define FL_IMPORT __attribute__((visibility("default")))
#else
#   pragma message Couldn't determine the platform. This might cause issues.

#   define FL_PLATFORM_UNKNOWN
#   define FL_EXPORT
#   define FL_IMPORT
#endif

#ifndef FL_CHECK_NDK_VER
#   define FL_CHECK_NDK_VER(ver) 0
#endif

// |------------------|
// | Feature checking |
// |------------------|

#ifdef __has_cpp_attribute
#   define FL_HAS_CPP_ATTRIBUTE(attr) __has_cpp_attribute(attr)
#else
#    define FL_HAS_CPP_ATTRIBUTE(attr) (0)
#endif

// "Assume" attribute
#ifndef FL_NO_ASSUME_ATTRIBUTE

#   if FL_CHECK_CPP_VER(FL_CPP23) || FL_HAS_CPP_ATTRIBUTE(assume)
#       define FL_ASSUME(expr) [[assume(expr)]]
#   endif

#   ifndef NAZARA_ASSUME
#       if defined(FL_COMPILER_CLANG)
#           define FL_ASSUME(expr) __builtin_assume(expr)
#       endif

#       if defined(FL_COMPILER_GCC)

            // __attribute__(assume) is supported starting with GCC 13
#           if __GNUC__ >= 13
#              define FL_ASSUME(expr) __attribute__(assume(expr))
#           endif
#       endif

#       if defined(FL_COMPILER_MSVC)
#           define FL_ASSUME(expr) __assume(expr)
#       endif
    #endif
#endif

// "Force inline" attribute
#ifndef FL_NO_FORCEINLINE_ATTRIBUTE

#   ifndef FL_FORCEINLINE

#       if defined(FL_COMPILER_CLANG) || defined(FL_COMPILER_GCC)
#           define FL_FORCEINLINE __attribute__((always_inline)) inline
#       endif

#       if defined(FL_COMPILER_MSVC)
#           define FL_FORCEINLINE __forceinline
#       endif

#   endif

#endif

// "Likely"/"Unlikely" attribute
#ifndef FL_NO_LIKELY_ATTRIBUTE

#   if FL_CHECK_CPP_VER(FL_CPP20) || FL_HAS_CPP_ATTRIBUTE(likely)
#       define FL_LIKELY(expr) (expr) [[likely]]
#   endif

#   if FL_CHECK_CPP_VER(FL_CPP20) || FL_HAS_CPP_ATTRIBUTE(unlikely)
#       define FL_UNLIKELY(expr) (expr) [[unlikely]]
#   endif

#   if defined(FL_COMPILER_CLANG) || defined(FL_COMPILER_GCC) || defined(FL_COMPILER_ICC)
#       ifndef FL_LIKELY
#           define FL_LIKELY(expr) (__builtin_expect(!!(expr), 1))
#       endif

#       ifndef FL_UNLIKELY
#           define FL_UNLIKELY(expr) (__builtin_expect(!!(expr), 0))
#       endif

#   endif

#endif

// Unreachable macro
#ifndef FL_NO_UNREACHABLE_MACRO

#   if defined(FL_COMPILER_CLANG) || defined(FL_COMPILER_GCC) || defined(FL_COMPILER_ICC)
#       define FL_UNREACHABLE() __builtin_unreachable()
#   elif defined(FL_COMPILER_MSVC)
#       define FL_UNREACHABLE() __assume(false)
#   endif

#endif

// Fallbacks

#ifndef FL_ASSUME
    #define FL_ASSUME(expr)
#endif

#ifndef FL_FORCEINLINE
    #define FL_FORCEINLINE inline
#endif

#ifndef FL_LIKELY
    #define FL_LIKELY(expr) (expr)
#endif

#ifndef FL_UNLIKELY
    #define FL_UNLIKELY(expr) (expr)
#endif

#ifndef FL_UNREACHABLE
    #define FL_UNREACHABLE() throw Fl::UnreachableError{};
#endif

// |----------------|
// | Arch detection |
// |----------------|
#ifndef FL_NO_ARCH_DETECTION

#   if defined(__arm__) || defined(__thumb__) || defined(__ARM_ARCH_7__) || defined(_M_ARM)
#       define FL_ARCH_arm
#   endif

#   if defined(__aarch64__) || defined(_M_ARM64) || defined(_M_ARM64EC) || defined(__arm64__)
#       define FL_ARCH_aarch64
#   endif

#   if defined(__amd64__) || defined(__amd64) || defined(__x86_64__) || defined(__x86_64) || defined(_M_AMD64) || defined (_M_X64)
#       define FL_ARCH_x86_64
#   endif

#   if defined(__i386__) || defined(_M_IX86) || defined(_X86_)
#       define FL_ARCH_x86
#   endif

#   if defined(__wasm32__)
#       define FL_ARCH_wasm32
#   endif

#   if defined(__wasm64__)
#       define FL_ARCH_wasm64
#   endif

#   if defined(FL_ARCH_arm)     \
     + defined(FL_ARCH_aarch64) \
     + defined(FL_ARCH_x86_64)  \
     + defined(FL_ARCH_x86)     \
     + defined(FL_ARCH_wasm32)  \
     + defined(FL_ARCH_wasm64)  \
     != 1
#error No or multiple arch detected! Please open an issue with details about your target system. You can define FL_NO_ARCH_DETECTION to bypass this error.

#endif

#endif

// |---------------------|
// | Unity build support |
// |---------------------|

#ifdef FL_UNITY_ID
#   define FL_ANONYMOUS_NAMESPACE FL_UNITY_ID
#   define FL_USE_ANONYMOUS_NAMESPACE using namespace FL_UNITY_ID;
#   define FL_ANONYMOUS_NAMESPACE_PREFIX(a) FL_UNITY_ID::a
#else
#   define FL_ANONYMOUS_NAMESPACE
#   define FL_USE_ANONYMOUS_NAMESPACE
#   define FL_ANONYMOUS_NAMESPACE_PREFIX(a) a
#endif

// |---------------|
// | Export macros |
// |---------------|

#if defined(FL_STATIC)
#   define FL_API
#elif defined(FL_BUILD)
#   define FL_API FL_EXPORT
#else
#   define FL_API FL_IMPORT
#endif

// |------------------|
// | Type definitions |
// |------------------|
#include <climits>
#include <cstddef>
#include <cstdint>

static_assert(sizeof(int8_t) ==  1, "Error: Expected int8_t to be 1 byte long.");
static_assert(sizeof(int16_t) == 2, "Error: Expected int16_t to be 2 bytes long.");
static_assert(sizeof(int32_t) == 4, "Error: Expected int32_t to be 4 bytes long.");
static_assert(sizeof(int64_t) == 8, "Error: Expected int64_t to be 8 bytes long.");

static_assert(sizeof(uint8_t) ==  1, "Error: Expected uint8_t to be 1 byte long.");
static_assert(sizeof(uint16_t) == 2, "Error: Expected uint16_t to be 2 bytes long.");
static_assert(sizeof(uint32_t) == 4, "Error: Expected uint32_t to be 4 bytes long.");
static_assert(sizeof(uint64_t) == 8, "Error: Expected uint64_t to be 8 bytes long.");

static_assert(sizeof(float) == 4, "Error: Expected float to be 4 bytes long.");
static_assert(sizeof(double) == 8, "Error: Expected double to be 8 bytes long.");

static_assert(CHAR_BIT == 8, "CHAR_BIT is expected to be 8");

namespace Fl {
    using Int8  = int8_t;
    using Int16 = int16_t;
    using Int32 = int32_t;
    using Int64 = int64_t;

    using UInt8  = uint8_t;
    using UInt16 = uint16_t;
    using UInt32 = uint32_t;
    using UInt64 = uint64_t;

    using Float32 = float;
    using Float64 = double;

    struct UnreachableError {};

    FL_API bool IsDebuggerAttached();
}

// |---------------|
// | Useful macros |
// |---------------|

#define FlPrefix(x, prefix) prefix ## a
#define FlPrefixMacro(x, prefix) FlPrefix(a, prefix)
#define FlSuffix(x, suffix) a ## suffix
#define FlSuffixMacro(x, suffix) FlSuffix(a, suffix)
#define FlStringify(s) #s
#define FlStringifyMacro(s) FlStringify(s)
#define FlUnused(x) (void)(x)

#if defined(FL_PLATFORM_WINDOWS)
#   define FlDebugBreak() __debugbreak()
#elif defined(FL_PLATFORM_POSIX)
#   if defined(FL_ARCH_arm) || defined(FL_ARCH_aarch64)
#       define FlDebugBreak() __builtin_trap()
#   else
#       define FlDebugBreak() asm("int $3")
#   endif
#else
#   define FlDebugBreak() {}
#endif

#endif // FL_PREREQUISITES_HPP
