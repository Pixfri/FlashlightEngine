// Copyright (C) 2025 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#ifndef FL_DATA_OWNERVALUE_HPP
#define FL_DATA_OWNERVALUE_HPP

#include <FlashlightEngine/Prerequisites.hpp>

#include <type_traits>

namespace Fl {
    template <typename T, T InvalidValue = T()>
    class OwnerValue {
    public:
        constexpr OwnerValue() = default;
        constexpr explicit OwnerValue(const T& value);
        constexpr explicit OwnerValue(T&& value) noexcept(std::is_nothrow_move_constructible_v<T>);
        constexpr OwnerValue(const OwnerValue&) = delete;
        constexpr OwnerValue(OwnerValue&& owner) noexcept;
        constexpr ~OwnerValue() = default;

        constexpr const T& Get() const;
        constexpr T& Get();
        constexpr bool IsValid() const;
        constexpr void Reset();

        constexpr OwnerValue& operator=(const OwnerValue&) = delete;
        constexpr OwnerValue& operator=(OwnerValue&& owner) noexcept;
        constexpr OwnerValue& operator=(const T& value);
        constexpr OwnerValue& operator=(T&& value) noexcept(std::is_nothrow_move_constructible_v<T>);

        constexpr operator const T&() const;
        constexpr operator T&();

    private:
        T m_Value = InvalidValue;
    };
}

#include <FlashlightEngine/Data/OwnerValue.inl>

#endif // FL_DATA_OWNERVALUE_HPP
