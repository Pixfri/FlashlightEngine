// Copyright (C) 2025 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#ifndef FL_DATASTRUCTURES_BITSET_HPP
#define FL_DATASTRUCTURES_BITSET_HPP

#include <FlashlightEngine/Prerequisites.hpp>

#include <iosfwd>
#include <initializer_list>
#include <vector>

namespace Fl {
    /**
     * @brief Represents a set of bits you can apply bitwise operations onto.
     */
    class Bitset {
    public:
        Bitset() = default;
        /**
         * @brief Creates a bitset with `bitCount` bits that all have the value `initialValue`.
         * @param bitCount The number of bits in the bitset.
         * @param initialValue The value to initially set the bits to.
         */
        inline explicit Bitset(U64 bitCount, bool initialValue = false);

        /**
         * @brief Creates a bitset using the value list provided (similarly to a std::vector).
         * @param values A list of values.
         */
        inline Bitset(std::initializer_list<bool> values);

        ~Bitset() = default;

        Bitset(const Bitset&) = default;
        Bitset(Bitset&&) noexcept = default;

        /**
         * @brief Get the size of the bitset.
         * @return The size of the bitset.
         */
        [[nodiscard]] inline U64 GetSize() const noexcept;

        /**
         * @brief Check if the bitset only contains zeros.
         * @return Whether the bitset only contains zeros.
         */
        [[nodiscard]] inline bool IsEmpty() const noexcept;

        /**
         * @brief Get the number of enabled bits in the bitset.
         * @return The number of bits set to 1 in the bitset.
         */
        [[nodiscard]] inline U64 GetEnabledBitCount() const noexcept;

        /**
         * @brief Get the number of disabled bits in the bitset.
         * @return The number of bits set to 0 in the bitset.
         */
        [[nodiscard]] inline U64 GetDisabledBitCount() const noexcept;

        /**
         * @brief Sets the value of a bit at a given index.
         * @param index The index of the bit to set.
         * @param value The value to set the bit to.
         */
        void SetBit(U64 index, bool value = true);

        /**
         * @brief Resizes the bitset to a given size, while keeping the values it already contains.
         * @param newSize The new size of the bitset.
         */
        inline void Resize(U64 newSize);

        /**
         * @brief Fills the bitset with 0s.
         */
        inline void Reset();

        /**
         * @brief Clears the bitset and sets its size to 0.
         */
        inline void Clear() noexcept;

        /**
         * @brief Bitset negation operator. All values in the bitset are set to their opposite.
         * @return The negated bitset.
         */
        Bitset operator~() const noexcept;

        /**
         * @brief Bitwise-and operator. Applies the & operator to every bit in the bitset.
         * @param bitset A bitset to compare to.
         * @return The result of the bitwise-and operation.
         */
        Bitset operator&(const Bitset& bitset) const noexcept;

        /**
         * @brief Bitwise-or operator. Applies the | operator to every bit in the bitset.
         * @param bitset A bitset to compare to.
         * @return The result of the bitwise-or operation.
         */
        Bitset operator|(const Bitset& bitset) const noexcept;

        /**
         * @brief Bitwise-xor operator. Applies the ^ operator to every bit in the bitset.
         * @param bitset A bitset to compare to.
         * @return The result of the bitwise-xor operation.
         */
        Bitset operator^(const Bitset& bitset) const noexcept;

        /**
         * @brief Bitwise-left-shift operator. Applies a shift to the left by a specific amount.
         * @param shift An amount to shift the bits by.
         * @return The result of the left-shifting operation.
         */
        Bitset operator<<(U64 shift) const;

        /**
         * @brief Bitwise-right-shift operator. Applies a shift to the right by a specific amount.
         * @param shift An amount to shift the bits by.
         * @return The result of the right-shifting operation.
         */
        Bitset operator>>(U64 shift) const;

        /**
         * @brief Bitwise-and assignment operator. Applies the & operator to every bit in this bitset.
         * @param bitset A bitset to compare to.
         * @return The result of the bitwise-and operation.
         */
        Bitset& operator&=(const Bitset& bitset) noexcept;

        /**
         * @brief Bitwise-or assignment operator. Applies the | operator to every bit in this bitset.
         * @param bitset A bitset to compare to.
         * @return The result of the bitwise-or operation.
         */
        Bitset& operator|=(const Bitset& bitset) noexcept;

        /**
         * @brief Bitwise-xor assignment operator. Applies the ^ operator to every bit in this bitset.
         * @param bitset A bitset to compare to.
         * @return The result of the bitwise-xor operation.
         */
        Bitset& operator^=(const Bitset& bitset) noexcept;

        /**
         * @brief Bitwise-left-shift assignment operator. Applies a shift to the left by a specific amount and assigns
         *        the returned bitset to itself.
         * @param shift An amount to shift the bits by.
         * @return The result of the left-shifting operation.
         */
        Bitset& operator<<=(U64 shift);

        /**
         * @brief Bitwise-right-shift assignment operator. Applies a shift to the right by a specific amount and assigns
         *        the returned bitset to itself.
         * @param shift An amount to shift the bits by.
         * @return The result of the right-shifting operation.
         */
        Bitset& operator>>=(U64 shift);

        /**
         * @brief Indexing operator. Gets the value at a specific index.
         * @param index The index of the value to retrieve.
         * @return The value at the given index.
         */
        inline bool operator[](U64 index) const noexcept;

        /**
         * @brief Bitset equality comparison operator.
         * @param bitset A bitset to compare to.
         * @return Whether this bitset is equal to the other or not.
         */
        inline bool operator==(const Bitset& bitset) const noexcept;

        /**
         * @brief Bitset inequality comparison operator.
         * @param bitset A bitset to compare to.
         * @return Whether this bitset is not equal to the other or not.
         */
        inline bool operator!=(const Bitset& bitset) const noexcept;

        /**
         * @brief Bitset stream output operator.
         * @param stream The stream to output to.
         * @param bitset The bitset to output.
         * @return The modified stream containing the bitset's string representation.
         */
        friend std::ostream& operator<<(std::ostream& stream, const Bitset& bitset);

        Bitset& operator=(const Bitset&) = default;
        Bitset& operator=(Bitset&&) noexcept = default;

    private:
        std::vector<bool> m_Bits{};
    };
}

#include <FlashlightEngine/DataStructures/Bitset.inl>

#endif // FL_DATASTRUCTURES_BITSET_HPP
