// Copyright (C) 2025 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#ifndef FL_MATH_CONSTANT_HPP
#define FL_MATH_CONSTANT_HPP

#include <FlashlightEngine/Prerequisites.hpp>

namespace Fl::Constants {
    /**
     * @brief The π constant.
     * @tparam T Type of Pi.
     */
    template <typename T>
    constexpr T Pi = static_cast<T>(3.14159265358979323846L);

    /**
     * @brief Constant to convert degrees to radians.
     * @tparam T Type of the conversion constant.
     */
    template <typename T>
    constexpr T DegToRadConstant = static_cast<T>(Pi<T> / 180);

    /**
     * @brief Constant to convert radians to degrees.
     * @tparam T Type of the conversion constant.
     */
    template <typename T>
    constexpr T RadToDegConstant = static_cast<T>(180 / Pi<T>);

    /**
     * @brief Golden ratio constant. Roughly equal to (1 + sqrt(5)) / 2.
     * @tparam T Type of the golden ratio.
     */
    template <typename T>
    constexpr T GoldenRatio = static_cast<T>(1.61803398874989484820L);
}

#endif // FL_MATH_CONSTANT_HPP
