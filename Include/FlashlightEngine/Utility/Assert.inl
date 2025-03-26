// Copyright (C) 2025 Jean "Pixfri" Letessier
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#include <FlashlightEngine/Utility/Assert.hpp>
#include <FlashlightEngine/Utility/DebugBreak.hpp>

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <utility>

namespace Fl {
    template <bool Dummy>
    FL_CONSTEXPR20 void AssertFailure() {
        if FL_IS_CONSTEVAL () {
            if constexpr (!Dummy) { //< just to make the throw dependent
                throw AssertionFailed{};
            }
        } else {
            std::fputs("Assertion failed\n", stderr);
            FlDebugBreak();
            assert(false);
            std::abort();
        }
    }

    template <bool Dummy, typename... Args>
    FL_CONSTEXPR20 void AssertFailure(const char* message, Args&&... args) {
        if FL_IS_CONSTEVAL () {
            if constexpr (!Dummy) { //< just to make the throw dependent
                throw AssertionFailed{};
            }
        } else {
            if constexpr (sizeof...(Args) > 0) {
                std::fputs("Assertion failed: ", stderr);
                std::fprintf(stderr, message, std::forward<Args>(args)...);
                std::fputc('\n', stderr);
            } else {
                std::fprintf(stderr, "Assertion failed: %s\n", message);
            }

            FlDebugBreak();
            assert(false);
            std::abort();
        }
    }

    template <bool Dummy, typename... Args>
    FL_CONSTEXPR20 void AssertFailureWithSource(const char* file, const unsigned int line, const char* message,
                                                Args&&... args) {
        if FL_IS_CONSTEVAL () {
            if constexpr (!Dummy) //< just to make the throw dependent
                throw AssertionFailed{};
        } else {
            if constexpr (sizeof...(Args) > 0) {
                std::fputs("Assertion failed: ", stderr);
                std::fprintf(stderr, message, std::forward<Args>(args)...);
                std::fprintf(stderr, " at %s:%d: ", file, line);
                std::fputc('\n', stderr);
            } else {
                std::fprintf(stderr, "Assertion failed: %s at %s:%d\n", message, file, line);
            }
            FlDebugBreak();
            assert(false);
            std::abort();
        }
    }

    // Defined after AssertFailure because otherwise GCC warns about undefined inline
    FL_CONSTEXPR20 FL_FORCEINLINE void Assert(const bool condition) {
        if FL_UNLIKELY (!condition) {
            AssertFailure();
        }
    }

    template <typename... Args>
    FL_CONSTEXPR20 FL_FORCEINLINE void Assert(const bool condition, const char* message, Args&&... args) {
        if FL_UNLIKELY (!condition) {
            AssertFailure(message, std::forward<Args>(args)...);
        }
    }

    template <typename... Args>
    FL_CONSTEXPR20 FL_FORCEINLINE void AssertWithSource(const bool condition, const char* file, unsigned int line,
                                                        const char* message, Args&&... args) {
        if FL_UNLIKELY (!condition)
            AssertFailureWithSource(file, line, message, std::forward<Args>(args)...);
    }
} // namespace Fl
