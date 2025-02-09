// Copyright (C) 2025 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

namespace Fl::MathUtils {
	template <typename T, typename CoefficientType>
	constexpr T Lerp(T min, T max, CoefficientType coeff) {
		static_assert(std::is_floating_point_v<CoefficientType>, "[Error] The coefficient must be a floating point value.");

		return static_cast<T>(static_cast<CoefficientType>(min) * (1 - coeff) + static_cast<CoefficientType>(max) * coeff);
	}

	template <typename T, U64 Size, typename CoefficientType>
	constexpr Vector<T, Size> Lerp(const Vector<T, Size>& min, const Vector<T, Size>& max, CoefficientType coeff) {
		static_assert(std::is_floating_point_v<CoefficientType>, "[Error] The coefficient must be a floating point value.");

		Vector<T, Size> result{};

		for (U64 i = 0; i < Size; i++) {
			result[i] = Lerp(min[i], max[i], coeff);
		}

		return result;
	}

	template <typename T>
	constexpr T Smoothstep(T value) noexcept {
		static_assert(std::is_floating_point_v<T>, "[Error] The interpolation type must be floating point.");
		FlAssert(value >= 0 && value <= 1, "The value must be between 0 and 1");

		return value * value * (3 - 2 * value);
	}

	template <typename T>
	constexpr T Smoothstep(T minThresh, T maxThresh, T value) noexcept {
		static_assert(std::is_floating_point_v<T>, "[Error] The interpolation type must be floating point.");
		FlAssert(maxThresh > minThresh, "The smoothstep's maximum threshold must be greater than the minimum one.");

		const T clampedVal = std::clamp((value - minThresh) / (maxThresh - minThresh), static_cast<T>(0), static_cast<T>(1));
		return Smoothstep(clampedVal);
	}

	template <typename T>
	constexpr T Smootherstep(T value) noexcept {
		static_assert(std::is_floating_point_v<T>, "[Error] The interpolation type must be floating point.");
		FlAssert(value >= 0 && value <= 1, "The value must be between 0 and 1.");

		return value * value * value * (value * (value * 6 - 15) + 10);
	}

	template <typename T>
	constexpr T Smootherstep(T minThresh, T maxThresh, T value) noexcept {
		static_assert(std::is_floating_point_v<T>, "[Error] The interpolation type must be floating point.");
		FlAssert(maxThresh > minThresh, "The smootherstep's maximum threshold must be greater than the minimum one.");

		const T clampedVal = std::clamp((value - minThresh) / (maxThresh - minThresh), static_cast<T>(0), static_cast<T>(1));
		return Smootherstep(clampedVal);
	}

	template <typename T>
	void ComputeOrthonormalBasis(const Vec3<T>& input, Vec3<T>& axis2, Vec3<T>& axis3) {
		static_assert(std::is_floating_point_v<T>, "[Error] Vectors must be of a floating-point type to compute an orthonormal basis from.");

		const T sign = std::copysign(static_cast<T>(1), input.Z());
		const T a    = static_cast<T>(-1) / (sign + input.Z());
		const T b    = input.X() * input.Y() * a;

		axis2 = Vec3<T>(static_cast<T>(1) + sign * input.X() * input.X() * a, sign * b, -sign * input.X());
		axis3 = Vec3<T>(b, sign + input.Y() * input.Y() * a, -input.Y());
	}

	inline std::vector<Vec3f> ComputeFibonacciSpherePoints(U64 pointCount) {
		std::vector<Vec3f> fiboPoints(pointCount);

		for (U64 i = 0; i < pointCount; i++) {
			const F32 theta  = 2.f * Constants::Pi<F32> *static_cast<F32>(i) / Constants::GoldenRatio<F32>;
			const F32 cosPhi = 1.f - 2.f * (static_cast<F32>(i) + 0.5f) / static_cast<F32>(pointCount);
			const F32 sinPhi = std::sqrt(1.f - cosPhi * cosPhi);

			fiboPoints[i] = Vec3f(std::cos(theta) * sinPhi, cosPhi, std::sin(theta) * sinPhi);
		}
		
		return fiboPoints;
	}
}
