// Copyright (C) 2025 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#ifndef FL_MATH_UTILS_HPP
#define FL_MATH_UTILS_HPP

#include <FlashlightEngine/Prerequisites.hpp>

#include <FlashlightEngine/Core/Assert.hpp>
#include <FlashlightEngine/Math/Constants.hpp>
#include <FlashlightEngine/Math/Vector.hpp>

#include <algorithm>
#include <type_traits>
#include <vector>

namespace Fl::MathUtils {
	/**
	 * @brief Computes the linear interpolation between two values, according to a coefficient.
	 * @tparam T Type of the values to compute the interpolation with.
	 * @tparam CoefficientType Type of the coefficient to apply.
	 * @param min Minimum value (lower bound).
	 * @param max Maximum value (upper bound).
	 * @param coeff Coefficient between 0 (returns `min`) and 1 (returns `max`).
	 * @return Computed linear interpolation between `min` and `max`.
	 */
	template <typename T, typename CoefficientType>
	constexpr T Lerp(T min, T max, CoefficientType coeff);

	/**
	 * @brief Computes the component-wise linear interpolation between two vectors, according to a coefficient.
	 * @tparam T Type of the vectors' data to compute the interpolation with.
	 * @tparam Size Size of the vectors to interpolate.
	 * @tparam CoefficientType Type of the coefficient to apply.
	 * @param min Minimum value (lower bound).
	 * @param max Maximum value (upper bound).
	 * @param coeff Coefficient between 0 (returns `min`) and 1 (returns `max`).
	 * @return Computed linear interpolation between `min` and `max`.
	 */
	template <typename T, U64 Size, typename CoefficientType>
	constexpr Vector<T, Size> Lerp(const Vector<T, Size>& min, const Vector<T, Size>& max, CoefficientType coeff);

	/**
	 * @brief Computes the cubic [Hermite interpolation](https://en.wikipedia.org/wiki/Hermite_interpolation) of a value.
	 *
	 *       1.0    |               |___
	 *              |           .-~"|
	 *              |         ,^    |
	 *              |        /      |
	 *              |       /       |
	 *              |      /        |
	 *              |    ,v         |
	 *       0.0 ___|,.-"           |
	 *              ^               ^
	 *             0.0             1.0
	 *
	 * @tparam T Type to compute the interpolation with.
	 * @param value Value to be interpolated (must be between 0 & 1).
	 * @return The interpolated value (between 0 & 1).
	 */
	template <typename T>
	constexpr T Smoothstep(T value) noexcept;

	/**
	 * @brief Computes the cubic [Hermite interpolation](https://en.wikipedia.org/wiki/Hermite_interpolation) between two thresholds.
	 *
	 * Any value below `minThresh` will return 0, and any above `maxThresh` will return 1. Between both thresholds, a smooth interpolation is performed.
	 *
	 *       1.0    |               |___
	 *              |           .-~"|
	 *              |         ,^    |
	 *              |        /      |
	 *              |       /       |
	 *              |      /        |
	 *              |    ,v         |
	 *       0.0 ___|,.-"           |
	 *              ^               ^
	 *          minThresh       maxThresh
	 *
	 * This is equivalent to [GLSL's smoothstep function](http://docs.gl/sl4/smoothstep).
	 * @tparam T Type to compute the interpolation with.
	 * @param minThresh Minimum threshold value.
	 * @param maxThresh Maximum threshold value.
	 * @param value Value to be interpolated.
	 * @return 0 if `value` is lower than `minThresh`.
	 * @return 1 if `value` is greater than `maxThresh`.
	 * @return The interpolated value (between 0 & 1) otherwise.
	 */
	template <typename T>
	constexpr T Smoothstep(T minThresh, T maxThresh, T value) noexcept;

	/**
	 * @brief Computes a quintic interpolation of a value.
	 *
	 * This is Ken Perlin's [smoothstep variation](https://en.wikipedia.org/wiki/Smoothstep#Variations), which produces a slightly smoother smoothstep.
	 * @tparam T Type to compute the interpolation with.
	 * @param value Value to be interpolated (must be between 0 & 1).
	 * @return The interpolated value (between 0 & 1).
	 */
	template <typename T>
	constexpr T Smootherstep(T value) noexcept;

	/**
	 * @brief Computes a quintic interpolation between two thresholds.
     *
     * This is Ken Perlin's [smoothstep variation](https://en.wikipedia.org/wiki/Smoothstep#Variations), which produces a slightly smoother smoothstep.
     * @tparam T Type to compute the interpolation with.
     * @param minThresh Minimum threshold value.
     * @param maxThresh Maximum threshold value.
     * @param value Value to be interpolated.
     * @return 0 if `value` is lower than `minThresh`.
     * @return 1 if `value` is greater than `maxThresh`.
     * @return The interpolated value (between 0 & 1) otherwise.
	 */
	template <typename T>
	constexpr T Smootherstep(T minThresh, T maxThresh, T value) noexcept;

	/**
	 * @brief Computes an [orthonormal basis from a single vector](https://graphics.pixar.com/library/OrthonormalB/), according to the right hand rule.
	 *
	 * Note that the example diagram below represents one possible solution; the vectors may not necessarily be in these directions relatively to the input.
	 *
	 *           axis3
	 *            ^
	 *            |
	 *            +---> axis2
	 *           /
	 *          v
	 *       input
	 *
	 * @tparam T Type of the basis' vectors.
	 * @param input Base vector from which to compute the basis. Must be normalized.
	 * @param axis2 Second vector of the computed basis.
	 * @param axis3 Third vector of the computed basis.
	 */
	template <typename T>
	void ComputeOrthonormalBasis(const Vec3<T>& input, Vec3<T>& axis2, Vec3<T>& axis3);

	/**
	 * @brief Computes points on the Fibonacci sphere, giving evenly distributed points around a unit sphere.
	 * @param pointCount Number of points to compute.
	 * @return Points on the Fibonacci sphere. All vectors are of length 1.
	 */
	inline std::vector<Vec3f> ComputeFibonacciSpherePoints(U64 pointCount);
}

#include <FlashlightEngine/Math/Utils.inl>

#endif // FL_MATH_UTILS_HPP
