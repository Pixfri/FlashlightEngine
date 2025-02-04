// Copyright (C) 2025 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#ifndef FL_DATASTRUCTURES_BITSET_HPP
#define FL_DATASTRUCTURES_BITSET_HPP

#include <FlashlightEngine/Prerequisites.hpp>

#include <algorithm>
#include <iosfwd>
#include <initializer_list>
#include <vector>

namespace Fl {
    class Bitset {
    public:
        Bitset() = default;
        inline explicit Bitset(U64 bitCount, bool initialValue = false);
        inline Bitset(std::initializer_list<bool> values);

        ~Bitset() = default;

        Bitset(const Bitset&) = default;
        Bitset(Bitset&&) noexcept = default;

        [[nodiscard]] inline U64 GetSize() const noexcept;

        [[nodiscard]] inline bool IsEmpty() const noexcept;
        [[nodiscard]] inline U64 GetEnabledBitCount() const noexcept;
        [[nodiscard]] inline U64 GetDisabledBitCount() const noexcept;
        void SetBit(U64 index, bool value = true);
        inline void Resize(U64 newSize);
        inline void Reset();
        inline void Clear() noexcept;

        Bitset operator~() const noexcept;
        Bitset operator&(const Bitset& bitset) const noexcept;
        Bitset operator|(const Bitset& bitset) const noexcept;
        Bitset operator^(const Bitset& bitset) const noexcept;
        Bitset operator<<(U64 shift) const;
        Bitset operator>>(U64 shift) const;
        Bitset& operator&=(const Bitset& bitset) noexcept;
        Bitset& operator|=(const Bitset& bitset) noexcept;
        Bitset& operator^=(const Bitset& bitset) noexcept;
        Bitset& operator<<=(U64 shift);
        Bitset& operator>>=(U64 shift);
        inline bool operator[](U64 index) const noexcept;
        inline bool operator==(const Bitset& bitset) const noexcept;
        inline bool operator!=(const Bitset& bitset) const noexcept;
        friend std::ostream& operator<<(std::ostream& stream, const Bitset& bitset);

        Bitset& operator=(const Bitset&) = default;
        Bitset& operator=(Bitset&&) noexcept = default;
    
    private:
        std::vector<bool> m_Bits{};
    };
}

#include <FlashlightEngine/DataStructures/Bitset.inl>

#endif // FL_DATASTRUCTURES_BITSET_HPP
