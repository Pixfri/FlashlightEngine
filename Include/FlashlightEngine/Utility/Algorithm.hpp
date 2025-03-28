// Copyright (C) 2025 Jean "Pixfri" Letessier
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#ifndef FL_UTILITY_ALGORITHM_HPP
#define FL_UTILITY_ALGORITHM_HPP

#include <FlashlightEngine/Prerequisites.hpp>
#include <FlashlightEngine/Utility/ConstantEvaluated.hpp>
#include <FlashlightEngine/Utility/TypeTraits.hpp>

#include <cmath>
#include <memory>
#include <type_traits>

#if __has_include(<version>)
#include <version>
#endif

// bit_cast support
#if defined(__cpp_lib_bit_cast) && __cpp_lib_bit_cast >= 201806L
#define FL_HAS_CONSTEXPR_BITCAST
#define FL_HAS_CONSTEXPR_BITCAST_STD
#define FL_CONSTEXPR_BITCAST constexpr
#elif FL_CHECK_MSVC_VER(1927) || FL_CHECK_CLANG_VER(1400) || FL_CHECK_GCC_VER(1100)
#define FL_HAS_CONSTEXPR_BITCAST
#define FL_HAS_CONSTEXPR_BITCAST_BUILTIN
#define FL_CONSTEXPR_BITCAST constexpr
#else
#define FL_CONSTEXPR_BITCAST
#endif

namespace Fl {
    /**
     * @brief Access a non-typed struct field by offset.
     * @param basePtr Pointer to the start of the struct
     * @param offset Offset to the field (as generated by offsetof or similar)
     * @return A pointer to the field of the asked type
     */
    template <typename T>
    [[nodiscard]] decltype(auto) AccessByOffset(void* basePtr, std::size_t offset) noexcept;
    /**
     * @brief Access a non-typed struct field by offset.
     * @param basePtr Pointer to the start of the struct
     * @param offset Offset to the field (as generated by offsetof or similar)
     * @return A pointer to the field of the asked type
     */
    template <typename T>
    [[nodiscard]] decltype(auto) AccessByOffset(const void* basePtr, std::size_t offset) noexcept;
    template <typename To, typename From>
    [[nodiscard]] FL_CONSTEXPR_BITCAST To BitCast(const From& value) noexcept;
    /**
     * @brief Returns the number of elements in a C-array
     * @return The number of elements
     *
     * @see CountOf
     */
    template <typename T, std::size_t N>
    [[nodiscard]] constexpr std::size_t CountOf(T (&name)[N]) noexcept;
    /**
     * @brief Returns the number of elements in a container
     * @param c Container with the member function "size()"
     * @return The number of elements
     *
     * @see CountOf
     */
    template <typename T>
    [[nodiscard]] constexpr std::size_t CountOf(const T& c) noexcept;
    template <std::size_t N>
    [[nodiscard]] constexpr std::size_t CountOf(const char (&str)[N]) noexcept;
    template <typename P, typename T>
    [[nodiscard]] FL_CONSTEXPR_BITCAST P IntegerToPointer(T ptrAsInt) noexcept;
    template <typename T, typename P>
    [[nodiscard]] FL_CONSTEXPR_BITCAST T PointerToInteger(P* ptr) noexcept;
    /**
     * @brief Helper function to retrieve a key in a map which has to exist.
     * @param map Map
     * @param key Key, has to exist in map
     * @return Value associated with key.
     */
    template <typename M, typename T>
    [[nodiscard]] FL_CONSTEXPR20 auto& Retrieve(M& map, const T& key) noexcept;
    /**
     * @brief Helper function to retrieve a key in a map which has to exist.
     * @param map Map
     * @param key Key, has to exist in map
     * @return Value associated with key.
     */
    template <typename M, typename T>
    [[nodiscard]] FL_CONSTEXPR20 const auto& Retrieve(const M& map, const T& key) noexcept;
    template <typename To, typename From>
    [[nodiscard]] FL_CONSTEXPR20 To SafeCast(From&& value) noexcept;
    template <typename From>
    [[nodiscard]] FL_CONSTEXPR20 auto SafeCaster(From&& value) noexcept;
    template <typename T, typename U>
    [[nodiscard]] std::unique_ptr<T> StaticUniquePointerCast(std::unique_ptr<U>&& ptr) noexcept;
    template <typename T>
    [[nodiscard]] constexpr auto UnderlyingCast(T value) noexcept -> std::underlying_type_t<T>;
}


#include <FlashlightEngine/Utility/Algorithm.inl>

#endif // FL_UTILITY_ALGORITHM_HPP
