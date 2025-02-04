// Copyright (C) 2025 Jean "Pixfri" Letessier
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

namespace Fl {
    inline Bitset::Bitset(const U64 bitCount, const bool initialValue)
        : m_Bits(bitCount, initialValue) {
    }

    inline Bitset::Bitset(std::initializer_list<bool> values)
        : m_Bits(values) {
    }

    inline U64 Bitset::GetSize() const noexcept {
        return m_Bits.size();
    }

    inline bool Bitset::IsEmpty() const noexcept {
        return (std::ranges::find(m_Bits, true)) == m_Bits.cend();
    }

    inline U64 Bitset::GetEnabledBitCount() const noexcept {
        return static_cast<U64>(std::ranges::count(m_Bits, true));
    }

    inline U64 Bitset::GetDisabledBitCount() const noexcept {
        return GetSize() - GetEnabledBitCount();
    }

    inline void Bitset::Resize(const U64 newSize) {
        m_Bits.resize(newSize);
    }

    inline void Bitset::Reset() {
        std::fill(m_Bits.begin(), m_Bits.end(), false);
    }

    inline void Bitset::Clear() noexcept {
        m_Bits.clear();
    }

    inline bool Bitset::operator[](const U64 index) const noexcept {
        return m_Bits[index];
    }

    inline bool Bitset::operator==(const Bitset& bitset) const noexcept {
        return std::equal(m_Bits.cbegin(), m_Bits.cend(), bitset.m_Bits.cbegin());
    }

    inline bool Bitset::operator!=(const Bitset& bitset) const noexcept {
        return !(*this == bitset);
    }
}
