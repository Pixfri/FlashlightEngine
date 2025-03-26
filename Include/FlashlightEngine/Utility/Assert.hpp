// Copyright (C) 2025 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#ifndef FL_UTILITY_ASSERT_HPP
#define FL_UTILITY_ASSERT_HPP

#include <FlashlightEngine/Prerequisites.hpp>
#include <FlashlightEngine/Utility/ConstantEvaluated.hpp>

#if !defined(FL_NO_ASSERT) && defined(FL_DEBUG)
#   define FlAssert(cond) if FL_UNLIKELY(!(cond)) Fl::AssertFailureWithSource(__FILE__, __LINE__, #cond)
#   define FlAssertMsg(cond, ...) if FL_UNLIKELY(!(cond)) Fl::AssertFailureWithSource(__FILE__, __LINE__, __VA_ARGS__)
#else
#   define FlAssert() do {} while (false)
#   define FlAssertMsg() do {} while (false)
#endif

namespace Fl {
    struct AssertionFailed {};

    FL_CONSTEXPR20 FL_FORCEINLINE void Assert(bool condition);
    template<typename... Args>
    FL_CONSTEXPR20 FL_FORCEINLINE void Assert(bool condition, const char* message, Args&&... args);
    template<typename... Args>
    FL_CONSTEXPR20 FL_FORCEINLINE void AssertWithSource(bool condition, const char* file, unsigned int line, const char* message, Args&&... args);

    template<bool Dummy = false>
    FL_CONSTEXPR20 void AssertFailure();
    template<bool Dummy = false, typename... Args>
    FL_CONSTEXPR20 void AssertFailure(const char* message, Args&&... args);
    template<bool Dummy = false, typename... Args>
    FL_CONSTEXPR20 void AssertFailureWithSource(const char* file, unsigned int line, const char* message, Args&&... args);
}

#include <FlashlightEngine/Utility/Assert.inl>

#endif // FL_UTILITY_ASSERT_HPP
