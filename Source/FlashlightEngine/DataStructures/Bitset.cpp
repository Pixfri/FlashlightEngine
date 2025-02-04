// Copyright (C) 2025 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#include <FlashlightEngine/DataStructures/Bitset.hpp>

#include <ostream>

namespace Fl {
    void Bitset::SetBit(const U64 index, const bool value) {
        if (index >= m_Bits.size()) {
            m_Bits.resize(index + 1);
        }

        m_Bits[index] = value;
    }

    Bitset Bitset::operator~() const noexcept {
        Bitset res = *this;

        for (auto bit : res.m_Bits) {
            bit = !bit;
        }

        return res;
    }

    Bitset Bitset::operator&(const Bitset& bitset) const noexcept {
        Bitset res(std::min(m_Bits.size(), bitset.GetSize()));
        std::copy_n(m_Bits.cbegin(), static_cast<std::ptrdiff_t>(res.GetSize()), res.m_Bits.begin());

        res &= bitset;

        return res;
    }

    Bitset Bitset::operator|(const Bitset& bitset) const noexcept {
        Bitset res(std::min(m_Bits.size(), bitset.GetSize()));
        std::copy_n(m_Bits.cbegin(), static_cast<std::ptrdiff_t>(res.GetSize()), res.m_Bits.begin());

        res |= bitset;

        return res;
    }

    Bitset Bitset::operator^(const Bitset& bitset) const noexcept {
        Bitset res(std::min(m_Bits.size(), bitset.GetSize()));
        std::copy_n(m_Bits.cbegin(), static_cast<std::ptrdiff_t>(res.GetSize()), res.m_Bits.begin());

        res ^= bitset;

        return res;
    }

    Bitset Bitset::operator<<(const U64 shift) const {
        Bitset res = *this;

        res <<= shift;

        return res;
    }

    Bitset Bitset::operator>>(const U64 shift) const {
        Bitset res = *this;
        res >>= shift;
        return res;
    }

    Bitset& Bitset::operator&=(const Bitset& bitset) noexcept {
        for (U64 bitIndex = 0; bitIndex < std::min(m_Bits.size(), bitset.GetSize()); ++bitIndex) {
            m_Bits[bitIndex] = m_Bits[bitIndex] && bitset[bitIndex];
        }

        return *this;
    }

    Bitset& Bitset::operator|=(const Bitset& bitset) noexcept {
        for (U64 bitIndex = 0; bitIndex < std::min(m_Bits.size(), bitset.GetSize()); ++bitIndex) {
            m_Bits[bitIndex] = m_Bits[bitIndex] || bitset[bitIndex];
        }

        return *this;
    }

    Bitset& Bitset::operator^=(const Bitset& bitset) noexcept {
        for (U64 bitIndex = 0; bitIndex < std::min(m_Bits.size(), bitset.GetSize()); ++bitIndex) {
            m_Bits[bitIndex] = m_Bits[bitIndex] ^ bitset[bitIndex];
        }

        return *this;
    }

    Bitset& Bitset::operator<<=(const U64 shift) {
        m_Bits.resize(m_Bits.size() + shift);

        return *this;
    }

    Bitset& Bitset::operator>>=(const U64 shift) {
        m_Bits.resize(m_Bits.size() - shift);

        return *this;
    }

    std::ostream& operator<<(std::ostream& stream, const Bitset& bitset) {
        stream << "[ " << bitset[0];

        for (U64 i = 1; i < bitset.GetSize(); ++i) {
            stream << ", " << bitset[i];
        }

        stream << " ]";

        return stream;
    }
}
