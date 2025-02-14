// Copyright (C) 2025 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#include <utility>

namespace Fl {
    template <typename T, T InvalidValue>
    constexpr OwnerValue<T, InvalidValue>::OwnerValue(const T& value)
        : m_Value(value) {
    }

    template <typename T, T InvalidValue>
    constexpr OwnerValue<T, InvalidValue>::OwnerValue(T&& value) noexcept(std::is_nothrow_move_constructible_v<T>)
        : m_Value(std::move(value)) {
    }

    template <typename T, T InvalidValue>
    constexpr OwnerValue<T, InvalidValue>::OwnerValue(OwnerValue&& owner) noexcept
        : m_Value{std::exchange(owner.m_Value, InvalidValue)} {
    }

    template <typename T, T InvalidValue>
    constexpr const T& OwnerValue<T, InvalidValue>::Get() const {
        return m_Value;
    }

    template <typename T, T InvalidValue>
    constexpr T& OwnerValue<T, InvalidValue>::Get() {
        return m_Value;
    }

    template <typename T, T InvalidValue>
    constexpr bool OwnerValue<T, InvalidValue>::IsValid() const {
        return m_Value != InvalidValue;
    }

    template <typename T, T InvalidValue>
    constexpr void OwnerValue<T, InvalidValue>::Reset() {
        m_Value = InvalidValue;
    }

    template <typename T, T InvalidValue>
    constexpr OwnerValue<T, InvalidValue>& OwnerValue<T, InvalidValue>::operator=(OwnerValue&& owner) noexcept {
        std::swap(m_Value, owner.m_Value);

        return *this;
    }

    template <typename T, T InvalidValue>
    constexpr OwnerValue<T, InvalidValue>& OwnerValue<T, InvalidValue>::operator=(const T& value) {
        m_Value = value;

        return *this;
    }

    template <typename T, T InvalidValue>
    constexpr OwnerValue<T, InvalidValue>& OwnerValue<T, InvalidValue>::operator=(T&& value) noexcept(std::is_nothrow_move_constructible_v<T>) {
        m_Value = std::move(value);

        return *this;
    }

    template <typename T, T InvalidValue>
    constexpr OwnerValue<T, InvalidValue>::operator const T&() const {
        return m_Value;
    }

    template <typename T, T InvalidValue>
    constexpr OwnerValue<T, InvalidValue>::operator T&() {
        return m_Value;
    }
}
