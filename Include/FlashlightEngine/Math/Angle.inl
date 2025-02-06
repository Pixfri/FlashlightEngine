// Copyright (C) 2025 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#include <FlashlightEngine/Core/Assert.hpp>

namespace Fl {
#pragma region Degrees
    template <typename T>
    constexpr Degrees<T>::Degrees(T val) noexcept
        : Value{val} {
    }

    template <typename T>
    constexpr Degrees<T>::Degrees(Radians<T> rad) noexcept
        : Value{rad * Constants::RadToDegConstant<T>} {
    }

    template <typename T>
    constexpr Degrees<T> Degrees<T>::operator-() const noexcept {
        return Degrees(-Value);
    }

    template <typename T>
    template <typename ValueType>
    constexpr Degrees<T> Degrees<T>::operator+(ValueType val) const noexcept {
        return Degrees(Value + static_cast<T>(val));
    }

    template <typename T>
    template <typename ValueType>
    constexpr Degrees<T> Degrees<T>::operator-(ValueType val) const noexcept {
        return Degrees(Value - static_cast<T>(val));
    }

    template <typename T>
    template <typename ValueType>
    constexpr Degrees<T> Degrees<T>::operator*(ValueType val) const noexcept {
        return Degrees(Value * static_cast<T>(val));
    }

    template <typename T>
    template <typename ValueType>
    constexpr Degrees<T> Degrees<T>::operator/(ValueType val) const noexcept {
        FlAssert(val != 0, "Cannot divide by 0.");
        return Degrees(Value / static_cast<T>(val));
    }

    template <typename T>
    template <typename ValueType>
    constexpr Degrees<T>& Degrees<T>::operator+=(ValueType val) const noexcept {
        Value += static_cast<T>(val);

        return *this;
    }

    template <typename T>
    template <typename ValueType>
    constexpr Degrees<T>& Degrees<T>::operator-=(ValueType val) const noexcept {
        Value -= static_cast<T>(val);

        return *this;
    }

    template <typename T>
    template <typename ValueType>
    constexpr Degrees<T>& Degrees<T>::operator*=(ValueType val) const noexcept {
        Value *= static_cast<T>(val);

        return *this;
    }

    template <typename T>
    template <typename ValueType>
    constexpr Degrees<T>& Degrees<T>::operator/=(ValueType val) const noexcept {
        FlAssert(val != 0, "Cannot divide by 0.");
        Value += static_cast<T>(val);

        return *this;
    }

    template <typename T>
    constexpr bool Degrees<T>::operator==(const Degrees& deg) const noexcept {
        return FloatMath::AreNearlyEqual(Value, deg.Value);
    }

    template <typename T>
    constexpr bool Degrees<T>::operator!=(const Degrees& deg) const noexcept {
        return !(*this == deg);
    }

    template <typename T>
    template <typename To>
    constexpr Degrees<T>::operator Degrees<To>() const noexcept {
        return Degrees<To>(static_cast<To>(Value));
    }

    template <typename T>
    template <typename To>
    constexpr Degrees<T>::operator Radians<To>() const noexcept {
        return Radians<To>(Degrees<To>(static_cast<To>(Value)));
    }
#pragma endregion Degrees

#pragma region Radians
    template <typename T>
    constexpr Radians<T>::Radians(T val) noexcept
        : Value{val} {
    }

    template <typename T>
    constexpr Radians<T>::Radians(Degrees<T> deg) noexcept
        : Value{deg * Constants::DegToRadConstant<T>} {
    }

    template <typename T>
    constexpr Radians<T> Radians<T>::operator-() const noexcept {
        return Radians(-Value);
    }

    template <typename T>
    template <typename ValueType>
    constexpr Radians<T> Radians<T>::operator+(ValueType val) const noexcept {
        return Radians(Value + static_cast<T>(val));
    }

    template <typename T>
    template <typename ValueType>
    constexpr Radians<T> Radians<T>::operator-(ValueType val) const noexcept {
        return Radians(Value - static_cast<T>(val));
    }

    template <typename T>
    template <typename ValueType>
    constexpr Radians<T> Radians<T>::operator*(ValueType val) const noexcept {
        return Radians(Value * static_cast<T>(val));
    }

    template <typename T>
    template <typename ValueType>
    constexpr Radians<T> Radians<T>::operator/(ValueType val) const noexcept {
        FlAssert(val != 0, "Cannot divide by 0.");
        return Radians(Value / static_cast<T>(val));
    }

    template <typename T>
    template <typename ValueType>
    constexpr Radians<T>& Radians<T>::operator+=(ValueType val) const noexcept {
        Value += static_cast<T>(val);

        return *this;
    }

    template <typename T>
    template <typename ValueType>
    constexpr Radians<T>& Radians<T>::operator-=(ValueType val) const noexcept {
        Value -= static_cast<T>(val);

        return *this;
    }

    template <typename T>
    template <typename ValueType>
    constexpr Radians<T>& Radians<T>::operator*=(ValueType val) const noexcept {
        Value *= static_cast<T>(val);

        return *this;
    }

    template <typename T>
    template <typename ValueType>
    constexpr Radians<T>& Radians<T>::operator/=(ValueType val) const noexcept {
        FlAssert(val != 0, "Cannot divide by 0.");
        Value += static_cast<T>(val);

        return *this;
    }

    template <typename T>
    constexpr bool Radians<T>::operator==(const Radians& rad) const noexcept {
        return FloatMath::AreNearlyEqual(Value, rad.Value);
    }

    template <typename T>
    constexpr bool Radians<T>::operator!=(const Radians& rad) const noexcept {
        return !(*this == rad);
    }

    template <typename T>
    template <typename To>
    constexpr Radians<T>::operator Radians<To>() const noexcept {
        return Radians<To>(static_cast<To>(Value));
    }

    template <typename T>
    template <typename To>
    constexpr Radians<T>::operator Degrees<To>() const noexcept {
        return Degrees<To>(Radians<To>(static_cast<To>(Value)));
    }
#pragma endregion Radians
}
