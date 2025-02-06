// Copyright (C) 2025 Jean "Pixfri" Letessier
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#ifndef FL_MATH_VECTOR_HPP
#define FL_MATH_VECTOR_HPP

#include <FlashlightEngine/Prerequisites.hpp>

#include <array>
#include <cmath>
#include <ostream>

namespace Fl {
    template <typename T, U64 Size>
    class Vector;

    template <typename T, U64 Size>
    std::ostream& operator<<(std::ostream& stream, const Vector<T, Size>& vec);

    /**
     * @brief Vector class representing a mathematical vector with `Size` elements.
     * @tparam T Type of the vector's members.
     * @tparam Size Vector's size.
     */
    template <typename T, U64 Size>
    class Vector {
    public:
        constexpr Vector() noexcept = default;

        constexpr explicit Vector(const Vector<T, Size + 1>& vec) noexcept;

        constexpr explicit Vector(const Vector<T, Size - 1>& vec, T val) noexcept;

        constexpr explicit Vector(T val) noexcept;

        template <typename... Args,
                  // There can't be more or less values than Size
                  typename = std::enable_if_t<sizeof...(Args) == Size>,
                  // Given values must be of a convertible type
                  typename = std::enable_if_t<(std::is_convertible_v<std::decay_t<Args>, T> && ...)>>
        constexpr explicit Vector(Args&&... args) noexcept;

        constexpr ~Vector() noexcept = default;

        Vector(const Vector&) noexcept = default;
        Vector(Vector&&) noexcept = default;

        [[nodiscard]] constexpr U64 GetSize() const noexcept;
        [[nodiscard]] constexpr const std::array<T, Size>& GetData() const noexcept;
        [[nodiscard]] constexpr std::array<T, Size>& GetData() noexcept;
        [[nodiscard]] constexpr const T* GetDataPtr() const noexcept;
        [[nodiscard]] constexpr T* GetDataPtr() noexcept;

        /**
         * @brief Gets the vector's X component.
         * @return Constant reference to the vector's X component.
         */
        constexpr const T& X() const noexcept;

        /**
         * @brief Gets the vector's X component.
         * @return Reference to the vector's X component.
         */
        constexpr T& X() noexcept;

        /**
         * @brief Gets the vector's X component.
         * @return Constant reference to the vector's X component.
         */
        constexpr const T& Y() const noexcept;

        /**
         * @brief Gets the vector's X component.
         * @return Reference to the vector's X component.
         */
        constexpr T& Y() noexcept;

        /**
         * @brief Gets the vector's X component.
         * @return Constant reference to the vector's X component.
         */
        constexpr const T& Z() const noexcept;

        /**
         * @brief Gets the vector's X component.
         * @return Reference to the vector's X component.
         */
        constexpr T& Z() noexcept;

        /**
         * @brief Gets the vector's X component.
         * @return Constant reference to the vector's X component.
         */
        constexpr const T& W() const noexcept;

        /**
         * @brief Gets the vector's X component.
         * @return Reference to the vector's X component.
         */
        constexpr T& W() noexcept;

        /**
         * @brief Computes the dot product (also known as the scalar or inner product) between the current vector and
         *        the given one. The dot product calculates the projection of one of the vectors onto the other; the
         *        order doesn't matter. On normalized vectors, the returned value represents the cosine of the angle
         *        (in radians) between them.
         * @tparam DotProductType The type of the result. For vectors of an integral type, it is defined to a large
         *                        unsigned integer type to avoid overflows.
         * @param vec Vector to compute the dot product with.
         * @return The result of the dot product.
         */
        template <typename DotProductType = std::conditional_t<std::is_integral_v<T>, U64, T>>
        constexpr DotProductType Dot(const Vector& vec) const noexcept;

        /**
         * @brief Computes the cross product (also called the vector product) between the current vector and the given
         *        one.
         * @param vec The vector to compute the cross product with.
         * @return Computed orthogonal vector.
         */
        constexpr Vector Cross(const Vector& vec) const noexcept;

        /**
         * @brief Reflects the current vector according to the given normal.
         * @param normal Direction of the normal vector to compute the reflection with.
         * @return The vector reflected according to the normal.
         */
        constexpr Vector Reflect(const Vector& normal) const noexcept;

        /**
         * @brief Computes the square length of the vector. This is faster than @ref ComputeLength because it does not
         *        involve a square root, and therefore should be preferred over @ref ComputeLength in most calculations.
         * @tparam SquareLengthType The type of the result. For vectors of an integral type, it is defined to a large
         *                          unsigned integer type to avoid overflows.
         * @return The squared length of the vector.
         */
        template <typename SquareLengthType = std::conditional_t<std::is_integral_v<T>, U64, T>>
        constexpr SquareLengthType ComputeSquaredLength() const noexcept;

        /**
         * @brief Computes the length of the vector. This is slower than @ref ComputeSquaredLength because it involves
         *        a square root.
         * @tparam LengthType The type of the result. For vectors of an integral type, it is defined to a large unsigned
         *                    integer type to avoid overflows.
         * @return The length of the vector.
         */
        template <typename LengthType = std::conditional_t<std::is_integral_v<T>, U64, T>>
        constexpr LengthType ComputeLength() const noexcept;

        /**
         * @brief Normalizes the current vector. Normalizing a vector makes it of length 1.
         * @tparam NormalizedType The type of the normalized vector's values. For vectors of an integral type, it is
         *                        defined to be a float.
         * @return Normalized vector.
         */
        template <typename NormalizedType = std::conditional_t<std::is_integral_v<T>, F32, T>>
        constexpr Vector<NormalizedType, Size> Normalize() const noexcept;

        /**
         * @brief Computes the linear interpolation of two vectors according to a coefficient.
         * @tparam LerpType Type of the interpolated vector's values. By default, it is the same as the current vector's.
         * @tparam CoeffType Type of the coefficient. For vectors of an integral type, it is defined to float; otherwise,
         *                   it is the same as the original vectors'.
         * @param vec Vector to be interpolated with.
         * @param coeff Coefficient between 0 (returns the current vector) and 1 (returns the other vector).
         * @return Linearly interpolated vector.
         */
        template <typename LerpType = T, typename CoeffType = std::conditional_t<std::is_integral_v<T>, F32, T>>
        constexpr Vector<LerpType, Size> Lerp(const Vector& vec, CoeffType coeff) const noexcept;

        /**
         * @brief Computes the normalized linear interpolation of two vectors according to a coefficient.
        * @tparam NormalizedType Type of the normalized interpolated vector's values. For vectors of an integral type,
        *                        it is defined to float; otherwise, it is the same as the original vectors'.
         * @tparam CoeffType Type of the coefficient. For vectors of an integral type, it is defined to float; otherwise,
         *                   it is the same as the original vectors'.
         * @param vec Vector to be interpolated with.
         * @param coeff Coefficient between 0 (returns the normalized current vector) and 1 (returns the normalized
         *              other vector).
         * @return Normalized linearly interpolated vector.
         */
        template <typename NormalizedType = std::conditional_t<std::is_integral_v<T>, F32, T>,
                  typename CoeffType = std::conditional_t<std::is_integral_v<T>, F32, T>>
        constexpr Vector<NormalizedType, Size> NLerp(const Vector& vec, CoeffType coeff) const noexcept;

        /**
         * @brief Checks for strict equality between the current vector & the given one. (May return errors because of
         *        floating-point imprecision).
         * @param vec Vector to be compared with.
         * @return Whether the two vectors are strictly equal to each other or not.
         */
        constexpr bool StrictlyEquals(const Vector& vec) const noexcept;

        /**
         * @brief Computes the unique hash value of the vector.
         * @param seed Value to use as a hash seed.
         * @return Vector's hash.
         */
        [[nodiscard]] constexpr U64 Hash(U64 seed = 0) const noexcept;

        /**
         * @brief Default copy assignment operator.
         * @return Reference to the copied vector.
         */
        constexpr Vector& operator=(const Vector&) noexcept = default;

        /**
         * @brief Default move assignment operator.
         * @return Reference to the moved vector.
         */
        constexpr Vector& operator=(Vector&&) noexcept = default;

        /**
         * @brief Vector negation operator. This unary minus negates the components of the vector, reversing its
         *        direction.
         * @return Negated vector.
         */
        constexpr Vector operator-() const noexcept;

        /**
         * @brief Element-wise vector-vector addition operator.
         * @param vec Vector to be added.
         * @return Result of the summed vectors.
         */
        constexpr Vector operator+(const Vector& vec) const noexcept;

        /**
         * Element-wise vector-value addition operator.
         * @param val Value to be added.
         * @return Result of the vector summed with the value.
         */
        constexpr Vector operator+(T val) const noexcept;

        /**
         * @brief Element-wise vector-vector subtraction operator.
         * @param vec Vector to be subtracted.
         * @return Result of the subtraction.
         */
        constexpr Vector operator-(const Vector& vec) const noexcept;

        /**
         * @brief Element-wise vector-value subtraction operator.
         * @param val Value to be subtracted.
         * @return Result of the subtraction.
         */
        constexpr Vector operator-(T val) const noexcept;

        /**
         * @brief Element-wise vector-vector multiplication operator (performs a [Hadamard product](https://en.wikipedia.org/wiki/Hadamard_product_(matrices)).
         * @param vec Vector to be multiplied by.
         * @return Result of the multiplication.
         */
        constexpr Vector operator*(const Vector& vec) const noexcept;

        /**
         * @brief Element-wise vector-value multiplication operator.
         * @param val Value to be multiplied by.
         * @return Result of the multiplication.
         */
        constexpr Vector operator*(T val) const noexcept;

        /**
         * @brief Element-wise vector-vector division operator.
         * @param vec Vector to be divided by.
         * @return Result of the division.
         */
        constexpr Vector operator/(const Vector& vec) const noexcept;

        /**
         * @brief Element-wise vector-value division operator.
         * @param val Value to be divided by.
         * @return Result of the division.
         */
        constexpr Vector operator/(T val) const noexcept;

        /**
         * @brief Element-wise vector-vector addition assignment operator.
         * @param vec Vector to be added.
         * @return Reference to the result of the summed vectors.
         */
        constexpr Vector& operator+=(const Vector& vec) const noexcept;

        /**
         * Element-wise vector-value addition assignment operator.
         * @param val Value to be added.
         * @return Reference to the result of the vector summed with the value.
         */
        constexpr Vector& operator+=(T val) const noexcept;

        /**
         * @brief Element-wise vector-vector subtraction assignment operator.
         * @param vec Vector to be subtracted.
         * @return Reference to the result of the subtraction.
         */
        constexpr Vector& operator-=(const Vector& vec) const noexcept;

        /**
         * @brief Element-wise vector-value subtraction assignment operator.
         * @param val Value to be subtracted.
         * @return Reference to the result of the subtraction.
         */
        constexpr Vector& operator-=(T val) const noexcept;

        /**
         * @brief Element-wise vector-vector multiplication assignment operator (performs a [Hadamard product](https://en.wikipedia.org/wiki/Hadamard_product_(matrices)).
         * @param vec Vector to be multiplied by.
         * @return Reference to the result of the multiplication.
         */
        constexpr Vector& operator*=(const Vector& vec) const noexcept;

        /**
         * @brief Element-wise vector-value multiplication assignment operator.
         * @param val Value to be multiplied by.
         * @return Reference to the result of the multiplication.
         */
        constexpr Vector& operator*=(T val) const noexcept;

        /**
         * @brief Element-wise vector-vector division assignment operator.
         * @param vec Vector to be divided by.
         * @return Reference to the result of the division.
         */
        constexpr Vector& operator/=(const Vector& vec) const noexcept;

        /**
         * @brief Element-wise vector-value division assignment operator.
         * @param val Value to be divided by.
         * @return Reference to the result of the division.
         */
        constexpr Vector& operator/=(T val) const noexcept;

        /**
         * @brief Element indexing operator.
         * @param index Element's index.
         * @return Constant reference to the element at the given index.
         */
        constexpr const T& operator[](U64 index) const noexcept;

        /**
         * @brief Element indexing operator.
         * @param index Element's index.
         * @return Reference to the element at the given index.
         */
        constexpr T& operator[](U64 index) noexcept;

        /**
         * @brief Vector equality comparison operator. Uses a near-equality check on floating-point types to take
         *        floating-point error into account.
         * @param vec Vector to be compared with.
         * @return Whether both vectors are nearly equal to each other or not.
         */
        constexpr bool operator==(const Vector& vec) const noexcept;

        /**
         * @brief Vector inequality comparison operator. Uses a near-equality check on floating-point types to take
         *        floating-point error into account.
         * @param vec Vector to be compared with.
         * @return Whether both vectors are different or not.
         */
        constexpr bool operator!=(const Vector& vec) const noexcept;

        /**
         * @brief Vector's value type conversion operator.
         * @tparam To Type to convert the vector's values to.
         * @result Vector with values of the new type.
         */
        template <typename To>
        constexpr explicit operator Vector<To, Size>() const noexcept;

        /**
         * @brief Stream output operator.
         * @param stream Stream to output to.
         * @param vec Vector to output.
         * @return The modified stream.
         */
        friend std::ostream& operator<<<>(std::ostream& stream, const Vector& vec);

    private:
        std::array<T, Size> m_Data{};
    };

    /**
     * @brief Element-wise value-vector addition operator (of the form val + vec).
     * @tparam T Type of the vector's data.
     * @tparam Size Size of the vector.
     * @param val Value to be added to the vector's elements.
     * @param vec Vector to be added.
     * @return Vector with the value added to its elements.
     */
    template <typename T, U64 Size>
    constexpr Vector<T, Size> operator+(T val, const Vector<T, Size>& vec) noexcept;

    /**
     * @brief Element-wise value-vector multiplication operator (of the form val * vec).
     * @tparam T Type of the vector's data.
     * @tparam Size Size of the vector.
     * @param val Value to multiply the vector's elements by.
     * @param vec Vector to be multiplied.
     * @return Vector with its elements multiplied by the value.
     */
    template <typename T, U64 Size>
    constexpr Vector<T, Size> operator*(T val, const Vector<T, Size>& vec) noexcept;

    // Deduction guides

    template <typename T, typename... Args>
    Vector(T, Args... args) -> Vector<T, sizeof...(Args) + 1>;

    // Aliases
    template <typename T>
    using Vec2 = Vector<T, 2>;

    template <typename T>
    using Vec3 = Vector<T, 3>;

    template <typename T>
    using Vec4 = Vector<T, 4>;

    using Vec2b = Vec2<U8>;
    using Vec3b = Vec3<U8>;
    using Vec4b = Vec4<U8>;

    using Vec2i = Vec2<I32>;
    using Vec3i = Vec3<I32>;
    using Vec4i = Vec4<I32>;

    using Vec2u = Vec2<U32>;
    using Vec3u = Vec3<U32>;
    using Vec4u = Vec4<U32>;

    using Vec2f = Vec2<F32>;
    using Vec3f = Vec3<F32>;
    using Vec4f = Vec4<F32>;

    using Vec2d = Vec2<F64>;
    using Vec3d = Vec3<F64>;
    using Vec4d = Vec4<F64>;

    namespace Axis {
        constexpr Vec3f X(1.0f, 0.0f, 0.0f);
        constexpr Vec3f Y(0.0f, 1.0f, 0.0f);
        constexpr Vec3f Z(0.0f, 0.0f, 1.0f);
    }

    /**
     * @brief Vector element fetching function for a constant lvalue reference.
     * @tparam I Index of the element.
     * @tparam T Type of the vector's elements.
     * @tparam Size Size of the vector.
     * @param vec Vector to get the element from.
     * @return Constant lvalue reference on the vector's element.
     */
    template <U64 I, typename T, U64 Size>
    constexpr const T& Get(const Vector<T, Size>& vec);

    /**
     * @brief Vector element fetching function for a non-constant lvalue reference.
     * @tparam I Index of the element.
     * @tparam T Type of the vector's elements.
     * @tparam Size Size of the vector.
     * @param vec Vector to get the element from.
     * @return Non-constant lvalue reference on the vector's element.
     */
    template <U64 I, typename T, U64 Size>
    constexpr T& Get(const Vector<T, Size>& vec);

    /**
     * @brief Vector element fetching function for a non-constant rvalue reference.
     * @tparam I Index of the element.
     * @tparam T Type of the vector's elements.
     * @tparam Size Size of the vector.
     * @param vec Vector to get the element from.
     * @return Non-constant rvalue reference on the vector's element.
     */
    template <U64 I, typename T, U64 Size>
    constexpr T&& Get(const Vector<T, Size>& vec);
}

/**
 * @brief Specialization of std::tuple_size for @ref Fl::Vector.
 * @tparam T Type of the vector's elements.
 * @tparam Size Size of the vector.
 */
template <typename T, Fl::U64 Size>
struct std::tuple_size<Fl::Vector<T, Size>> : std::integral_constant<Fl::U64, Size> {};

/**
 * @brief Specialization of std::tuple_element for @ref Fl::Vector.
 * @tparam I Index of the element.
 * @tparam T Type of the vector's elements.
 * @tparam Size Size of the vector.
 */
template <std::size_t I, typename T, Fl::U64 Size>
struct std::tuple_element<I, Fl::Vector<T, Size>> {
    using type = T;
};

/**
 * @brief Specialization of std::hash for @ref Fl::Vector.
 * @tparam T Type of the vector's elements.
 * @tparam Size Size of the vector.
 */
template <typename T, Fl::U64 Size>
struct std::hash<Fl::Vector<T, Size>> {
    /**
     * @brief Computes the hash of the given vector.
     * @param vec Vector to compute the hash for.
     * @return Vector's hash value.
     */
    constexpr std::size_t operator()(const Fl::Vector<T, Size>& vec) const noexcept;
};

/**
 * @brief Specialization of std::equal_to for @ref Fl::Vector. This performs a strict equality check.
 * @tparam T Type of the vector's elements.
 * @tparam Size Size of the vector.
 */
template <typename T, Fl::U64 Size>
struct std::equal_to<Fl::Vector<T, Size>> {
    /**
     * @brief Checks that the two given vectors are strictly equal to each other.
     * @param vec1 First vector to compare.
     * @param vec2 Second vector to compare.
     * @return Whether both vectors are equal to each other or not.
     */
    constexpr std::size_t operator()(const Fl::Vector<T, Size>& vec1, const Fl::Vector<T, Size>& vec2) const noexcept;
};

/**
 * @brief Specialization of std::less for @ref Fl::Vector.
 * @tparam T Type of the vector's elements.
 * @tparam Size Size of the vector.
 */
template <typename T, Fl::U64 Size>
struct std::less<Fl::Vector<T, Size>> {
    /**
     * @brief Checks that the first given vector is strictly less than the other.
     * @param vec1 First vector to compare.
     * @param vec2 Second vector to compare.
     * @return Whether the first vector is strictly less than the other or not.
     */
    constexpr bool operator()(const Fl::Vector<T, Size>& vec1, const Fl::Vector<T, Size>& vec2) const noexcept;
};

#include <FlashlightEngine/Math/Vector.inl>

#endif // FL_MATH_VECTOR_HPP
