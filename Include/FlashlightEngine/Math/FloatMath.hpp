// Copyright (C) 2025 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#ifndef FL_MATH_FLOATMATH_HPP
#define FL_MATH_FLOATMATH_HPP

#include <FlashlightEngine/Prerequisites.hpp>

#include <algorithm>
#include <limits>

namespace Fl {
    template <typename T, U64 Size>
    class Vector;

    template <typename T, U64 Width, U64 Height>
    class Matrix;

    template <typename T>
    class Quaternion;

    namespace FloatMath {
        /**
         * @brief Compares two floating point values to see if they are nearly equal or not.
         * @tparam T Type of the two values to compare. Must be a floating point type.
         * @tparam ToleranceType The type of the tolerance constant. Must be a floating point type.
         * @param val1 The first value.
         * @param val2 The second value.
         * @param absoluteTolerance The tolerance to compare the value with.
         * @return Whether the two values are nearly equal or not.
         */
        template <typename T, typename ToleranceType = T>
        constexpr bool AreNearlyEqual(
            T val1,
            T val2,
            ToleranceType absoluteTolerance = std::numeric_limits<ToleranceType>::epsilon()
        ) noexcept;

        /**
         * @brief Compares two floating point vectors to see if they are nearly equal or not.
         * @tparam T Type of the two vectors' values to compare. Must be a floating point type.
         * @tparam ToleranceType The type of the tolerance constant. Must be a floating point type.
         * @param vec1 The first vector.
         * @param vec2 The second vector.
         * @param absoluteTolerance The tolerance to compare the value with.
         * @return Whether the two vectors are nearly equal or not.
         */
        template <typename T, U64 Size, typename ToleranceType = T>
        constexpr bool AreNearlyEqual(
            Vector<T, Size> vec1,
            Vector<T, Size> vec2,
            ToleranceType absoluteTolerance = std::numeric_limits<ToleranceType>::epsilon()
        ) noexcept;

        /**
         * @brief Compares two floating point matrices to see if they are nearly equal or not.
         * @tparam T Type of the two matrices' values to compare. Must be a floating point type.
         * @tparam ToleranceType The type of the tolerance constant. Must be a floating point type.
         * @param mat1 The first matrix.
         * @param mat2 The second matrix.
         * @param absoluteTolerance The tolerance to compare the value with.
         * @return Whether the two matrices are nearly equal or not.
         */
        template <typename T, U64 Width, U64 Height, typename ToleranceType = T>
        constexpr bool AreNearlyEqual(
            Matrix<T, Width, Height> mat1,
            Matrix<T, Width, Height> mat2,
            ToleranceType absoluteTolerance = std::numeric_limits<ToleranceType>::epsilon()
        ) noexcept;

        /**
         * @brief Compares two floating point quaternions to see if they are nearly equal or not.
         * @tparam T Type of the two quaternions' values to compare. Must be a floating point type.
         * @tparam ToleranceType The type of the tolerance constant. Must be a floating point type.
         * @param quat1 The first quaternion.
         * @param quat2 The second quaternion.
         * @param absoluteTolerance The tolerance to compare the value with.
         * @return Whether the two quaternions are nearly equal or not.
         */
        template <typename T, typename ToleranceType = T>
        constexpr bool AreNearlyEqual(
            Quaternion<T> quat1,
            Quaternion<T> quat2,
            ToleranceType absoluteTolerance = std::numeric_limits<ToleranceType>::epsilon()
        ) noexcept;
    }
}

#include <FlashlightEngine/Math/FloatMath.inl>

#endif // FL_MATH_FLOATMATH_HPP
