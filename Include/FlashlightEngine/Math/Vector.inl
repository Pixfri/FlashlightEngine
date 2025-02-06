// Copyright (C) 2025 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#include <FlashlightEngine/Core/Assert.hpp>
#include <FlashlightEngine/Math/FloatMath.hpp>

#include <algorithm>

namespace Fl {
    template <typename T, U64 Size>
    constexpr Vector<T, Size>::Vector(const Vector<T, Size + 1>& vec) noexcept {
        for (U64 i = 0; i < Size; i++) {
            m_Data[i] = vec[i];
        }
    }

    template <typename T, U64 Size>
    constexpr Vector<T, Size>::Vector(const Vector<T, Size - 1>& vec, T val) noexcept {
        for (U64 i = 0; i < Size - 1; i++) {
            m_Data[i] = vec[i];
        }

        m_Data.back() = val;
    }

    template <typename T, U64 Size>
    constexpr Vector<T, Size>::Vector(T val) noexcept {
        for (T& element : m_Data) {
            element = val;
        }
    }

    template <typename T, U64 Size>
    template <typename... Args, typename, typename>
    constexpr Vector<T, Size>::Vector(Args&&... args) noexcept
        : m_Data{static_cast<T>(args)...} {
    }

    template <typename T, U64 Size>
    constexpr U64 Vector<T, Size>::GetSize() const noexcept {
        return Size;
    }

    template <typename T, U64 Size>
    constexpr const std::array<T, Size>& Vector<T, Size>::GetData() const noexcept {
        return m_Data;
    }

    template <typename T, U64 Size>
    constexpr std::array<T, Size>& Vector<T, Size>::GetData() noexcept {
        return m_Data;
    }

    template <typename T, U64 Size>
    constexpr const T* Vector<T, Size>::GetDataPtr() const noexcept {
        return m_Data.data();
    }

    template <typename T, U64 Size>
    constexpr T* Vector<T, Size>::GetDataPtr() noexcept {
        return m_Data.data();
    }

    template <typename T, U64 Size>
    constexpr const T& Vector<T, Size>::X() const noexcept {
        static_assert(
            Size >= 1, "[Error] Fl::Vector needs to have at least a size of 1 to have its X component retrieved.");
        return m_Data[0];
    }

    template <typename T, U64 Size>
    constexpr T& Vector<T, Size>::X() noexcept {
        return const_cast<T&>(static_cast<const Vector*>(this)->X());
    }

    template <typename T, U64 Size>
    constexpr const T& Vector<T, Size>::Y() const noexcept {
        static_assert(
            Size >= 2, "[Error] Fl::Vector needs to have at least a size of 2 to have its Y component retrieved.");
        return m_Data[1];
    }

    template <typename T, U64 Size>
    constexpr T& Vector<T, Size>::Y() noexcept {
        return const_cast<T&>(static_cast<const Vector*>(this)->Y());
    }

    template <typename T, U64 Size>
    constexpr const T& Vector<T, Size>::Z() const noexcept {
        static_assert(
            Size >= 3, "[Error] Fl::Vector needs to have at least a size of 3 to have its Z component retrieved.");
        return m_Data[2];
    }

    template <typename T, U64 Size>
    constexpr T& Vector<T, Size>::Z() noexcept {
        return const_cast<T&>(static_cast<const Vector*>(this)->Z());
    }

    template <typename T, U64 Size>
    constexpr const T& Vector<T, Size>::W() const noexcept {
        static_assert(
            Size >= 4, "[Error] Fl::Vector needs to have at least a size of 4 to have its W component retrieved.");
        return m_Data[3];
    }

    template <typename T, U64 Size>
    constexpr T& Vector<T, Size>::W() noexcept {
        return const_cast<T&>(static_cast<const Vector*>(this)->W());
    }

    template <typename T, U64 Size>
    template <typename DotProductType>
    constexpr DotProductType Vector<T, Size>::Dot(const Vector& vec) const noexcept {
        DotProductType result{};
        for (U64 i = 0; i < Size; i++) {
            result += static_cast<DotProductType>(m_Data[i]) * static_cast<DotProductType>(vec[i]);
        }

        return result;
    }

    template <typename T, U64 Size>
    constexpr Vector<T, Size> Vector<T, Size>::Cross(const Vector& vec) const noexcept {
        static_assert(Size == 3, "[Error] Both vectors must be 3 dimensional to compute their cross product.");
        static_assert(std::is_signed_v<T>,
                      "[Error] The cross product of vectors can only be computed with vectors of a signed type.");

        Vector result;

        result.m_Data[0] = m_Data[1] * vec.m_Data[2] - m_Data[2] * vec.m_Data[1];
        result.m_Data[1] = -(m_Data[0] * vec.m_Data[2] - m_Data[2] * vec.m_Data[0]);
        result.m_Data[2] = m_Data[0] * vec.m_Data[1] - m_Data[1] * vec.m_Data[0];

        return result;
    }

    template <typename T, U64 Size>
    constexpr Vector<T, Size> Vector<T, Size>::Reflect(const Vector& normal) const noexcept {
        static_assert(std::is_signed_v<T>,
                      "[Error] The reflection of a vector can only be computed with vectors of a signed type.");

        return (*this - normal * static_cast<T>(Dot(normal)) * 2);
    }

    template <typename T, U64 Size>
    template <typename SquareLengthType>
    constexpr SquareLengthType Vector<T, Size>::ComputeSquaredLength() const noexcept {
        return static_cast<SquareLengthType>(Dot(*this));
    }

    template <typename T, U64 Size>
    template <typename LengthType>
    constexpr LengthType Vector<T, Size>::ComputeLength() const noexcept {
        return std::sqrt(static_cast<LengthType>(ComputeSquaredLength()));
    }

    template <typename T, U64 Size>
    template <typename NormalizedType>
    constexpr Vector<NormalizedType, Size> Vector<T, Size>::Normalize() const noexcept {
        static_assert(std::is_floating_point_v<NormalizedType>,
                      "[Error] The normalized vector's type must be floating-point.");

        const NormalizedType sqLength = ComputeSquaredLength<NormalizedType>();
        Vector<NormalizedType, Size> result(*this);

        if (sqLength != static_cast<NormalizedType>(0)) {
            result /= std::sqrt(sqLength);
        }

        return result;
    }

    template <typename T, U64 Size>
    template <typename LerpType, typename CoeffType>
    constexpr Vector<LerpType, Size> Vector<T, Size>::Lerp(const Vector& vec, CoeffType coeff) const noexcept {
        static_assert(std::is_floating_point_v<CoeffType>,
                      "[Error] The linear interpolation coefficient type must be floating-point.");
        FlAssert(coeff >= 0 && coeff <= 1, "The interpolation coefficient must be between 0 & 1.");

        const Vector<CoeffType, Size> convertedThis(*this);
        const Vector<CoeffType, Size> lerpVec = convertedThis + (Vector<CoeffType, Size>(vec) - convertedThis) * coeff;

        return Vector<LerpType, Size>(lerpVec);
    }

    template <typename T, U64 Size>
    template <typename NormalizedType, typename CoeffType>
    constexpr Vector<NormalizedType, Size> Vector<T, Size>::NLerp(const Vector& vec, CoeffType coeff) const noexcept {
        return Lerp<NormalizedType>(vec, coeff).Normalize();
    }

    template <typename T, U64 Size>
    constexpr bool Vector<T, Size>::StrictlyEquals(const Vector& vec) const noexcept {
        return std::equal(m_Data.cbegin(), m_Data.cend(), vec.m_Data.cbegin());
    }

    template <typename T, U64 Size>
    constexpr U64 Vector<T, Size>::Hash(U64 seed) const noexcept {
        std::hash<T> hasher{};

        for (const T& element : m_Data) {
            seed ^= hasher(element) + 0x9e3779b9 + (seed << 6u) + (seed >> 2u);
        }

        return seed;
    }

    template <typename T, U64 Size>
    constexpr Vector<T, Size> Vector<T, Size>::operator-() const noexcept {
        Vector result;

        for (U64 i = 0; i < Size; i++) {
            result[i] = -m_Data[i];
        }

        return result;
    }

    template <typename T, U64 Size>
    constexpr Vector<T, Size> Vector<T, Size>::operator+(const Vector& vec) const noexcept {
        Vector result = *this;

        result += vec;

        return result;
    }

    template <typename T, U64 Size>
    constexpr Vector<T, Size> Vector<T, Size>::operator+(T val) const noexcept {
        Vector result = *this;

        result += val;

        return result;
    }

    template <typename T, U64 Size>
    constexpr Vector<T, Size> Vector<T, Size>::operator-(const Vector& vec) const noexcept {
        Vector result = *this;

        result -= vec;

        return result;
    }

    template <typename T, U64 Size>
    constexpr Vector<T, Size> Vector<T, Size>::operator-(T val) const noexcept {
        Vector result = *this;

        result -= val;

        return result;
    }

    template <typename T, U64 Size>
    constexpr Vector<T, Size> Vector<T, Size>::operator*(const Vector& vec) const noexcept {
        Vector result = *this;

        result *= vec;

        return result;
    }

    template <typename T, U64 Size>
    constexpr Vector<T, Size> Vector<T, Size>::operator*(T val) const noexcept {
        Vector result = *this;

        result *= val;

        return result;
    }

    template <typename T, U64 Size>
    constexpr Vector<T, Size> Vector<T, Size>::operator/(const Vector& vec) const noexcept {
        Vector result = *this;

        result /= vec;

        return result;
    }

    template <typename T, U64 Size>
    constexpr Vector<T, Size> Vector<T, Size>::operator/(T val) const noexcept {
        Vector result = *this;

        result /= val;

        return result;
    }

    template <typename T, U64 Size>
    constexpr Vector<T, Size>& Vector<T, Size>::operator+=(const Vector& vec) const noexcept {
        for (U64 i = 0; i < Size; i++) {
            m_Data[i] += vec[i];
        }

        return *this;
    }

    template <typename T, U64 Size>
    constexpr Vector<T, Size>& Vector<T, Size>::operator+=(T val) const noexcept {
        for (T& element : m_Data) {
            element += val;
        }

        return *this;
    }

    template <typename T, U64 Size>
    constexpr Vector<T, Size>& Vector<T, Size>::operator-=(const Vector& vec) const noexcept {
        for (U64 i = 0; i < Size; i++) {
            m_Data[i] -= vec[i];
        }

        return *this;
    }

    template <typename T, U64 Size>
    constexpr Vector<T, Size>& Vector<T, Size>::operator-=(T val) const noexcept {
        for (T& element : m_Data) {
            element -= val;
        }

        return *this;
    }

    template <typename T, U64 Size>
    constexpr Vector<T, Size>& Vector<T, Size>::operator*=(const Vector& vec) const noexcept {
        for (U64 i = 0; i < Size; i++) {
            m_Data[i] *= vec[i];
        }

        return *this;
    }

    template <typename T, U64 Size>
    constexpr Vector<T, Size>& Vector<T, Size>::operator*=(T val) const noexcept {
        for (T& element : m_Data) {
            element *= val;
        }

        return *this;
    }

    template <typename T, U64 Size>
    constexpr Vector<T, Size>& Vector<T, Size>::operator/=(const Vector& vec) const noexcept {
        if constexpr (std::is_integral_v<T>)
            FlAssert(std::find(vec.m_Data.cbegin(), vec.m_Data.cend(), 0) == vec.m_Data.cend(),
                     "Division by 0 is undefined.");

        for (U64 i = 0; i < Size; i++) {
            m_Data[i] /= vec[i];
        }

        return *this;
    }

    template <typename T, U64 Size>
    constexpr Vector<T, Size>& Vector<T, Size>::operator/=(T val) const noexcept {
        if constexpr (std::is_integral_v<T>)
            FlAssert(val != 0, "Division by 0 is undefined.");

        for (T& element : m_Data) {
            element /= val;
        }

        return *this;
    }

    template <typename T, U64 Size>
    constexpr const T& Vector<T, Size>::operator[](const U64 index) const noexcept {
        return m_Data[index];
    }

    template <typename T, U64 Size>
    constexpr T& Vector<T, Size>::operator[](const U64 index) noexcept {
        return m_Data[index];
    }

    template <typename T, U64 Size>
    constexpr bool Vector<T, Size>::operator==(const Vector& vec) const noexcept {
        if constexpr (std::is_floating_point_v<T>) {
            return FloatMath::AreNearlyEqual(*this, vec);
        }

        return StrictlyEquals(vec);
    }

    template <typename T, U64 Size>
    constexpr bool Vector<T, Size>::operator!=(const Vector& vec) const noexcept {
        return !(*this == vec);
    }

    template <typename T, U64 Size>
    template <typename To>
    constexpr Vector<T, Size>::operator Vector<To, Size>() const noexcept {
        Vector<To, Size> result;

        for (U64 i = 0; i < Size; i++) {
            result[i] = static_cast<To>(m_Data[i]);
        }

        return result;
    }

    template <typename T, U64 Size>
    std::ostream& operator<<(std::ostream& stream, const Vector<T, Size>& vec) {
        using PrintType = std::conditional_t<std::is_same_v<T, U8>, I32, T>;

        stream << "[ " << static_cast<PrintType>(vec[0]);

        for (U64 i = 1; i < Size; i++) {
            stream << ", " << static_cast<PrintType>(vec[i]);
        }

        stream << " ]";

        return stream;
    }

    template <typename T, U64 Size>
    constexpr Vector<T, Size> operator+(T val, const Vector<T, Size>& vec) noexcept {
        return vec + val;
    }

    template <typename T, U64 Size>
    constexpr Vector<T, Size> operator*(T val, const Vector<T, Size>& vec) noexcept {
        return vec * val;
    }

    template <U64 I, typename T, U64 Size>
    constexpr const T& Get(const Vector<T, Size>& vec) {
        static_assert(I < Size);
        return vec[I];
    }

    template <U64 I, typename T, U64 Size>
    constexpr T& Get(const Vector<T, Size>& vec) {
        static_assert(I < Size);
        return vec[I];
    }

    template <U64 I, typename T, U64 Size>
    constexpr T&& Get(const Vector<T, Size>& vec) {
        static_assert(I < Size);
        return std::move(vec[I]);
    }
}

template <typename T, Fl::U64 Size>
constexpr std::size_t std::hash<Fl::Vector<T, Size>>::operator()(const Fl::Vector<T, Size>& vec) const noexcept {
    return vec.Hash();
}

template <typename T, Fl::U64 Size>
constexpr std::size_t std::equal_to<Fl::Vector<T, Size>>::operator()(const Fl::Vector<T, Size>& vec1,
                                                                     const Fl::Vector<T, Size>& vec2) const noexcept {
    return vec1.StrictlyEquals(vec2);
}

template <typename T, Fl::U64 Size>
constexpr bool std::less<Fl::Vector<T, Size>>::operator()(const Fl::Vector<T, Size>& vec1,
                                                          const Fl::Vector<T, Size>& vec2) const noexcept {
    for (Fl::U64 i = 0; i < Size; i++) {
        if (vec1[i] == vec2[i]) {
            continue;
        }

        return (vec1[i] < vec2[i]);
    }

    return false;
}
