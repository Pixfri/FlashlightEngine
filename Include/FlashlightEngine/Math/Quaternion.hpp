// Copyright (C) 2025 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#ifndef FL_MATH_QUATERNION_HPP
#define FL_MATH_QUATERNION_HPP

#include <FlashlightEngine/Prerequisites.hpp>

#include <FlashlightEngine/Math/Vector.hpp>

namespace Fl {
    template <typename T>
    struct Degrees;

    template <typename T>
    struct Radians;

    template <typename T, U64 Width, U64 Height>
    class Matrix;

    template <typename T>
    using Mat4 = Matrix<T, 4, 4>;

    template <typename T>
    class Quaternion;

    template <typename T>
    std::ostream& operator<<(std::ostream& stream, const Quaternion<T>& quat);

    /**
     * @brief Quaternion representing a rotation in a 3D space. Quaternions are used to avoid [gimbal locks](https://en.wikipedia.org/wiki/Gimbal_lock)
     *        present with Euler angles.
     * @tparam T Type of the values held by the quaternion.
     */
    template <typename T>
    class Quaternion {
        static_assert(std::is_floating_point_v<T>, "[Error] Fl::Quaternion's type must be floating point.");

    public:
        constexpr Quaternion(T w, T x, T y, T z) noexcept;
        constexpr Quaternion(Radians<T> angle, const Vec3<T>& axis) noexcept;
        constexpr Quaternion(Radians<T> angle, T axisX, T axisY, T axisZ) noexcept;
        constexpr Quaternion(const Quaternion&) noexcept = default;
        constexpr Quaternion(Quaternion&&) noexcept = default;
        constexpr ~Quaternion() noexcept = default;

        constexpr T W() const noexcept;
        constexpr T X() const noexcept;
        constexpr T Y() const noexcept;
        constexpr T Z() const noexcept;

        /**
         * @brief Creates a quaternion representing an identity transformation.
         * @return Identity quaternion.
         */
        static constexpr Quaternion Identity() noexcept;

        /**
         * @brief Computes the dot product between quaternions.
         * @param quat Quaternion to compute the dot product with.
         * @return Quaternions' dot product.
         */
        constexpr T Dot(const Quaternion& quat) const noexcept;
        /**
         * @brief Computes the norm of the quaternion.
         * @note This requires a square root, which is expensive. As such, @ref ComputeSquaredNorm should be preferred.
         *       This function should be use only when the actual length is needed.
         * @return Quaternion's norm.
         */
        constexpr T ComputeNorm() const noexcept;
        /**
         * @brief Computes the squared norm of the quaternion. The squared norm is equal to the addition of all
         *        components (real & complexes) squared.
         * @note This requires no square root. As such, it should be preferred over @ref ComputeNorm.
         * @return Quaternion's squared norm.
         */
        constexpr T ComputeSquaredNorm() const noexcept;
        /**
         * @brief Computes the normalized quaternion to make it a unit one. A unit quaternion is also called a
         *        [versor](https://en.wikipedia.org/wiki/Versor).
         * @return Normalized quaternion.
         */
        constexpr Quaternion Normalize() const noexcept;
        /**
         * @brief Computes the linear interpolation between quaternions, according to a coefficient.
         * @param quat Quaternion to be interpolated with.
         * @param coeff Coefficient between 0 (returns the current quaternion) and 1 (returns the given quaternion).
         * @return Linearly interpolated quaternion.
         */
        constexpr Quaternion Lerp(const Quaternion& quat, T coeff) const noexcept;
        /**
         * @brief Computes the normalized linear interpolation between quaternions, according to a coefficient.
         * @param quat Quaternion to be interpolated with.
         * @param coeff Coefficient between 0 (returns the normalized current quaternion) and 1 (returns the normalized
         *              given quaternion).
         * @return Normalized linearly interpolated quaternion.
         */
        constexpr Quaternion NLerp(const Quaternion& quat, T coeff) const noexcept;
        /**
         * @brief Computes the spherical linear interpolation between normalized quaternions, according to a coefficient.
         * @note Both quaternions (the current & given ones) must be normalized before calling this function.
         * @param quat Quaternion to be interpolated with. Must be normalized.
         * @param coeff Coefficient between 0 (returns the current quaternion) and 1 (returns the given quaternion).
         * @return Spherically interpolated quaternion.
         */
        constexpr Quaternion SLerp(const Quaternion& quat, T coeff) const noexcept;
        /**
         * @brief Computes the conjugate of the quaternion. A quaternion's conjugate is simply computed by multiplying
         *        the complex components by -1.
         * @return Quaternion's conjugate.
         */
        constexpr Quaternion Conjugate() const noexcept;
        /**
         * @brief Computes the inverse (or reciprocal) of the quaternion. Inverting a quaternion consists of dividing
         *        the components of the conjugate by the squared norm.
         * @return Quaternion's inverse.
         */
        constexpr Quaternion Inverse() const noexcept;
        /**
         * @brief Computes the rotation matrix represented by the quaternion. This operation automatically scales the
         *        matrix so that it returns a unit one.
         * @return Rotation matrix.
         */
        constexpr Mat4<T> ComputeMatrix() const noexcept;

        /**
         * @brief Default copy assignment operator.
         * @return Reference to the copied quaternion.
         */
        constexpr Quaternion& operator=(const Quaternion&) noexcept = default;
        /**
         * @brief Default move assignment operator.
         * @return Reference to the moved quaternion.
         */
        constexpr Quaternion& operator=(Quaternion&&) noexcept = default;
        /**
         * @brief Quaternions multiplication operator.
         * @param quat Quaternion to be multiplied by.
         * @return Result of the multiplied quaternions.
         */
        constexpr Quaternion operator*(const Quaternion& quat) const noexcept;
        /**
         * @brief Quaternions multiplication assignment operator.
         * @param quat Quaternion to be multiplied by.
         * @return Reference to the modified original quaternion.
         */
        constexpr Quaternion& operator*=(const Quaternion& quat) noexcept;
        /**
         * @brief Quaternion equality comparison operator. This uses a near-equality algorithm to take floating-point
         *        error into account.
         * @param quat Quaternion to be compared to.
         * @return Whether the quaternions are nearly equal or not.
         */
        constexpr bool operator==(const Quaternion& quat) const noexcept;
        /**
         * @brief Quaternion inequality comparison operator. This uses a near-equality algorithm to take floating-point
         *        error into account.
         * @param quat Quaternion to be compared to.
         * @return Whether the quaternions not equal or if they are.
         */
        constexpr bool operator!=(const Quaternion& quat) const noexcept;
        /**
         * @brief Matrix conversion operator; computes the rotation matrix represented by the quaternion.
         * @return Rotation matrix.
         */
        constexpr operator Mat4<T>() const noexcept;
        /**
         * @brief Output stream operator.
         * @param stream Stream to output to.
         * @param quat Quaternion to be output.
         * @return The modified stream.
         */
        friend std::ostream& operator<<<>(std::ostream& stream, const Quaternion& quat);

    private:
        constexpr Quaternion Lerp(const Quaternion& quat, T currentCoeff, T otherCoeff) const noexcept;

        T m_Real{};
        Vec3<T> m_Complexes{};
    };

    /**
     * @brief Quaternion-vector multiplication. Rotates the vector by the rotation represented by the quaternion.
     *        The quaternion must be normalized.
     * @tparam T Type of the quaternion's & vector's data.
     * @param quat Quaternion to rotate by.
     * @param vec Vector to be rotated.
     * @return Rotated vector.
     */
    template <typename T>
    constexpr Vec3<T> operator*(const Quaternion<T>& quat, const Vec3<T>& vec) noexcept;

    /**
     * @brief Vector-quaternion multiplication. Rotates the vector by the inverse rotation represented by the quaternion.
     *        The quaternion must be normalized.
     * @tparam T Type of the vector's & quaternion's data.
     * @param vec Vector to be rotated.
     * @param quat Quaternion to rotate by.
     * @return Rotated vector.
     */
    template <typename T>
    constexpr Vec3<T> operator*(const Vec3<T>& vec, const Quaternion<T>& quat) noexcept;

    // Deduction guide

    template <typename T>
    Quaternion(Degrees<T>, ...) -> Quaternion<T>;

    // Aliases
    using Quaternionf = Quaternion<F32>;
    using Quaterniond = Quaternion<F64>;

    template <U64 Index, typename T>
    constexpr T get(const Quaternion<T>& quat) noexcept {
        static_assert(Index < 4);

        if constexpr (Index == 0)      return quat.W();
        else if constexpr (Index == 1) return quat.X();
        else if constexpr (Index == 2) return quat.Y();
        else                           return quat.Z();
    }
}

/**
 * @brief Specialization of std::tuple_size for Fl::Quaternion.
 * @tparam T Type of the quaternion's data.
 */
template <typename T>
struct std::tuple_size<Fl::Quaternion<T>> : std::integral_constant<std::size_t, 4> {};

/**
 * @brief Specialization of std::tuple_element for Fl::Quaternion.
 * @tparam Index Index of the element (0, 1, 2 or 3 for the W, X, Y or Z component, respectively).
 * @tparam T Type of the quaternion's data.
 */
template <std::size_t Index, typename T>
struct std::tuple_element<Index, Fl::Quaternion<T>> { using type = T; };


#include <FlashlightEngine/Math/Quaternion.inl>

#endif // FL_MATH_QUATERNION_HPP
