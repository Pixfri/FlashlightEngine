// Copyright (C) 2025 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#include <FlashlightEngine/Core/Assert.hpp>
#include <FlashlightEngine/Math/FloatMath.hpp>

namespace Fl {
    template <typename T>
    constexpr Quaternion<T>::Quaternion(T w, T x, T y, T z) noexcept
        : m_Real(w), m_Complexes(x, y, z) {
    }

    template <typename T>
    constexpr Quaternion<T>::Quaternion(const Radians<T> angle, const Vec3<T>& axis) noexcept {
        const T halfAngle = angle.Value / 2;
        const T sinAngle = std::sin(halfAngle);

        m_Real = std::cos(halfAngle);
        m_Complexes = axis * sinAngle;
    }

    template <typename T>
    constexpr Quaternion<T>::Quaternion(const Radians<T> angle, const T axisX, const T axisY, const T axisZ) noexcept
        : Quaternion(angle, Vec3<T>(axisX, axisY, axisZ)) {
    }

    template <typename T>
    constexpr T Quaternion<T>::W() const noexcept {
        return m_Real;
    }

    template <typename T>
    constexpr T Quaternion<T>::X() const noexcept {
        return m_Complexes.X();
    }

    template <typename T>
    constexpr T Quaternion<T>::Y() const noexcept {
        return m_Complexes.Y();
    }

    template <typename T>
    constexpr T Quaternion<T>::Z() const noexcept {
        return m_Complexes.Z();
    }

    template <typename T>
    constexpr Quaternion<T> Quaternion<T>::Identity() noexcept {
        return Quaternion(1, 0, 0, 0);
    }

    template <typename T>
    constexpr T Quaternion<T>::Dot(const Quaternion& quat) const noexcept {
        return m_Real * quat.m_Real + m_Complexes.Dot(quat.m_Complexes);
    }

    template <typename T>
    constexpr T Quaternion<T>::ComputeNorm() const noexcept {
        return std::sqrt(ComputeSquaredNorm());
    }

    template <typename T>
    constexpr T Quaternion<T>::ComputeSquaredNorm() const noexcept {
        return Dot(*this);
    }

    template <typename T>
    constexpr Quaternion<T> Quaternion<T>::Normalize() const noexcept {
        Quaternion result = *this;

        const T invNorm = 1 / ComputeNorm();
        result.m_Real *= invNorm;
        result.m_Complexes *= invNorm;

        return result;
    }

    template <typename T>
    constexpr Quaternion<T> Quaternion<T>::Lerp(const Quaternion& quat, T coeff) const noexcept {
        FlAssert(coeff >= 0 && coeff <= 1, "The interpolation coefficient must be a number between 0 and 1.");

        const T currentCoeff = 1 - coeff;
        return Lerp(quat, currentCoeff, coeff);
    }

    template <typename T>
    constexpr Quaternion<T> Quaternion<T>::NLerp(const Quaternion& quat, T coeff) const noexcept {
        FlAssert(coeff >= 0 && coeff <= 1, "The interpolation coefficient must be a number between 0 and 1.");

        const T currentCoeff = 1 - coeff;
        // A rotation may be represented by two opposite quaternions; should the dot product between those be negative,
        // one quaternion must be negated. This is made by negating the coefficient.
        const T otherCoeff = Dot(quat) > 0 ? coeff : -coeff;

        return Lerp(quat, currentCoeff, otherCoeff).Normalize();
    }

    template <typename T>
    constexpr Quaternion<T> Quaternion<T>::SLerp(const Quaternion& quat, T coeff) const noexcept {
        FlAssert(coeff >= 0 && coeff <= 1, "The interpolation coefficient must be a number between 0 and 1.");
        FlAssert(FloatMath::AreNearlyEqual(ComputeSquaredNorm(), static_cast<T>(1)) &&
                 FloatMath::AreNearlyEqual(quat.ComputeSquaredNorm(), static_cast<T>(1)),
                 "Both quaternions must be normalized for a spherical linear interpolation to be performed.");

        const T cosAngle = Dot(quat);

        T currentCoeff{};
        T otherCoeff{};

        // Checking the angle between the quaternions; if the angle is sufficient, perform an actual spherical
        // interpolation.
        if (std::abs(cosAngle) < static_cast<T>(0.99999)) {
            const T angle = std::acos(cosAngle);
            const T invSinAngle = 1 / std::sin(angle);

            currentCoeff = std::sin((1 - coeff) * angle) * invSinAngle;
            otherCoeff = std::sin(coeff * angle) * invSinAngle;
        } else {
            // If the angle is small enough, a simple linear interpolation is performed.
            currentCoeff = 1 - coeff;
            otherCoeff = coeff;
        }

        // Negating one quaternion if the angle is negative.
        otherCoeff = cosAngle > 0 ? otherCoeff : -otherCoeff;
        return Lerp(quat, currentCoeff, otherCoeff);
    }

    template <typename T>
    constexpr Quaternion<T> Quaternion<T>::Conjugate() const noexcept {
        Quaternion result = *this;
        result.m_Complexes = -m_Complexes;

        return result;
    }

    template <typename T>
    constexpr Quaternion<T> Quaternion<T>::Inverse() const noexcept {
        Quaternion result = *this;
        const T sqNorm = ComputeSquaredNorm();

        if (sqNorm > 0) {
            const T invSqNorm = 1 / sqNorm;

            result.m_Real *= invSqNorm;
            result.m_Complexes *= invSqNorm;
        }

        return result;
    }

    // @formatter:off
    // clang-format off
    template <typename T>
    constexpr Mat4<T> Quaternion<T>::ComputeMatrix() const noexcept {
		const T invSqNorm = 1 / ComputeSquaredNorm();

		const T xx = (2 * m_Complexes.X() * m_Complexes.X()) * invSqNorm;
        const T yy = (2 * m_Complexes.Y() * m_Complexes.Y()) * invSqNorm;
        const T zz = (2 * m_Complexes.Z() * m_Complexes.Z()) * invSqNorm;

		const T xy = (2 * m_Complexes.X() * m_Complexes.Y()) * invSqNorm;
		const T xz = (2 * m_Complexes.X() * m_Complexes.Z()) * invSqNorm;
		const T yz = (2 * m_Complexes.Y() * m_Complexes.Z()) * invSqNorm;

		const T xw = (2 * m_Complexes.X() * m_Real) * invSqNorm;
		const T yw = (2 * m_Complexes.Y() * m_Real) * invSqNorm;
		const T zw = (2 * m_Complexes.Z() * m_Real) * invSqNorm;

		return Mat4<T>(1 - yy - zz,       xy - zw,           xz + yw,           static_cast<T>(0),
			           xy + zw,           1 - xx - zz,       yz - xw,           static_cast<T>(0),
			           xz - yw,           yz + xw,  1 -      xx - yy,           static_cast<T>(0),
			           static_cast<T>(0), static_cast<T>(0), static_cast<T>(0), static_cast<T>(1));
    }
    // @formatter:on
    // clang-format on

    template <typename T>
    constexpr Quaternion<T> Quaternion<T>::operator*(const Quaternion& quat) const noexcept {
        Quaternion result = *this;
        result *= quat;

        return result;
    }

    // @formatter:off
    // clang-format off
    template <typename T>
    constexpr Quaternion<T>& Quaternion<T>::operator*=(const Quaternion& quat) noexcept {
		const Quaternion copy = *this;

		m_Real = copy.m_Real          * quat.m_Real
			   - copy.m_Complexes.X() * quat.m_Complexes.X()
               - copy.m_Complexes.Y() * quat.m_Complexes.Y()
               - copy.m_Complexes.Z() * quat.m_Complexes.Z();

		m_Complexes.X() = copy.m_Real          * quat.m_Complexes.X()
			            + copy.m_Complexes.X() * quat.m_Real
                        + copy.m_Complexes.Y() * quat.m_Complexes.Z()
                        - copy.m_Complexes.Z() * quat.m_Complexes.Y();

		m_Complexes.Y() = copy.m_Real          * quat.m_Complexes.Y()
			            - copy.m_Complexes.X() * quat.m_Complexes.Z()
			            + copy.m_Complexes.Y() * quat.m_Real
			            + copy.m_Complexes.Z() * quat.m_Complexes.X();

		m_Complexes.Z() = copy.m_Real * quat.m_Complexes.Z()
			            + copy.m_Complexes.X() * quat.m_Complexes.Y()
                        - copy.m_Complexes.Y() * quat.m_Complexes.X()
                        + copy.m_Complexes.Z() * quat.m_Real;

		return *this;
    }
    // @formatter:on
    // clang-format on

    template <typename T>
    constexpr bool Quaternion<T>::operator==(const Quaternion& quat) const noexcept {
        return FloatMath::AreNearlyEqual(*this, quat);
    }

    template <typename T>
    constexpr bool Quaternion<T>::operator!=(const Quaternion& quat) const noexcept {
        return !(*this == quat);
    }

    template <typename T>
    constexpr Quaternion<T>::operator Mat4<T>() const noexcept {
        return ComputeMatrix();
    }

    template <typename T>
    std::ostream& operator<<(std::ostream& stream, const Quaternion<T>& quat) {
        stream << "[ " << quat.W() << ", " << quat.X() << ", " << quat.Y() << ", " << quat.Z() << " ]";
        return stream;
    }

    // @formatter:off
    // clang-format off
    template <typename T>
    constexpr Quaternion<T> Quaternion<T>::Lerp(const Quaternion& quat, T currentCoeff, T otherCoeff) const noexcept {
        return Quaternion(m_Real          * currentCoeff + quat.m_Real          * otherCoeff,
                          m_Complexes.X() * currentCoeff + quat.m_Complexes.X() * otherCoeff,
                          m_Complexes.Y() * currentCoeff + quat.m_Complexes.Y() * otherCoeff,
                          m_Complexes.Z() * currentCoeff + quat.m_Complexes.Z() * otherCoeff);
    }
    // @formatter:on
    // clang-format on

    template <typename T>
    constexpr Vec3<T> operator*(const Quaternion<T>& quat, const Vec3<T>& vec) noexcept {
        // Adapted from https://fgiesen.wordpress.com/2019/02/09/rotating-a-single-vector-using-a-quaternion/

        const Vec3<T> quatVec(quat.X(), quat.Y(), quat.Z());
        const Vec3<T> doubleQuatVecCross = static_cast<T>(2) * quatVec.Cross(vec);
        return vec + quat.W() * doubleQuatVecCross + quatVec.Cross(doubleQuatVecCross);
    }

    template <typename T>
    constexpr Vec3<T> operator*(const Vec3<T>& vec, const Quaternion<T>& quat) noexcept {
        // Adapted from https://fgiesen.wordpress.com/2019/02/09/rotating-a-single-vector-using-a-quaternion/

        const Vec3<T> quatVec(quat.X(), quat.Y(), quat.Z());
        const Vec3<T> doubleQuatVecCross = static_cast<T>(2) * vec.Cross(quatVec);
        return vec + quat.W() * doubleQuatVecCross + doubleQuatVecCross.Cross(quatVec);
    }
}
