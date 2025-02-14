// Copyright (C) 2025 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#include <cmath>

namespace Fl::FloatMath {
    template <typename T, typename ToleranceType>
    constexpr bool AreNearlyEqual(T val1, T val2, ToleranceType absoluteTolerance) noexcept {
        static_assert(std::is_floating_point_v<T>, "[Error] T must be a floating point type.");
        static_assert(std::is_floating_point_v<ToleranceType>, "[Error] ToleranceType must be a floating point type.");

        const T absoluteDifference = std::abs(val1 - val2);
        return (absoluteDifference <= static_cast<T>(absoluteTolerance)
            * std::max(static_cast<T>(1), std::max(std::abs(val1), std::abs(val2)))
        );
    }

    template <typename T, U64 Size, typename ToleranceType>
    constexpr bool AreNearlyEqual(
        Vector<T, Size> vec1,
        Vector<T, Size> vec2,
        ToleranceType absoluteTolerance
    ) noexcept {
        for (U64 i = 0; i < Size; ++i) {
            if (!AreNearlyEqual(vec1[i], vec2[i], absoluteTolerance)) {
                return false;
            }
        }

        return true;
    }

    template <typename T, U64 Width, U64 Height, typename ToleranceType>
    constexpr bool AreNearlyEqual(
        Matrix<T, Width, Height> mat1,
        Matrix<T, Width, Height> mat2,
        ToleranceType absoluteTolerance
    ) noexcept {
        for (U64 i = 0; i < Width * Height; ++i) {
            if (!AreNearlyEqual(mat1[i], mat2[i], absoluteTolerance)) {
                return false;
            }
        }

        return true;
    }

    template <typename T, typename ToleranceType>
    constexpr bool AreNearlyEqual(Quaternion<T> quat1, Quaternion<T> quat2, ToleranceType absoluteTolerance) noexcept {
        if (!AreNearlyEqual(quat1.W(), quat2.W(), absoluteTolerance) ||
            !AreNearlyEqual(quat1.X(), quat2.X(), absoluteTolerance) ||
            !AreNearlyEqual(quat1.Y(), quat2.Y(), absoluteTolerance) ||
            !AreNearlyEqual(quat1.Z(), quat2.Z(), absoluteTolerance)) {
            return false;
        }

        return true;
    }
}
