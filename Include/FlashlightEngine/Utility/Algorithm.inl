// Copyright (C) 2025 Jean "Pixfri" Letessier
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#include <FlashlightEngine/Utility/Algorithm.hpp>

#include <FlashlightEngine/Utility/Assert.hpp>

#include <cassert>

#ifdef FL_HAS_CONSTEXPR_BITCAST_STD
#   include <bit>
#elif !defined(FL_HAS_CONSTEXPR_BITCAST_BUILTIN)
#   include <cstring>
#endif

#ifdef FL_COMPILER_MSVC
#   pragma intrinsic(_BitScanForward)

#   if defined(FL_ARCH_aarch64) || defined(FL_ARCH_x86_64)
#       pragma intrinsic(_BitScanForward64)
#   endif

#   if defined(FL_ARCH_x86) || defined(FL_ARCH_x86_64)
#       include <intrin.h>
#   endif
#endif

namespace Fl {
    namespace Detail {
        template <typename From>
        class SafeCaster {
            static_assert(std::is_reference_v<From>);

        public:
            template <typename T>
            constexpr SafeCaster(T&& from) : m_from(std::forward<T>(from)) {
            }

            template <typename To>
            constexpr operator To() const {
                return SafeCast<To>(m_from);
            }

        private:
            From m_from;
        };
    } // namespace Detail

    template <typename T>
    decltype(auto) AccessByOffset(void* basePtr, const std::size_t offset) noexcept {
        if constexpr (std::is_lvalue_reference_v<T>) {
            return *reinterpret_cast<std::remove_reference_t<T>*>(static_cast<UInt8*>(basePtr) + offset);
        } else if constexpr (std::is_pointer_v<T>) {
            return reinterpret_cast<T>(static_cast<UInt8*>(basePtr) + offset);
        } else {
            static_assert(AlwaysFalse<T>(), "AccessByOffset requires a reference or a pointer type.");
        }
    }

    template <typename T>
    decltype(auto) AccessByOffset(const void* basePtr, const std::size_t offset) noexcept {
        static_assert(std::is_lvalue_reference_v<T> || std::is_pointer_v<T>);

        if constexpr (std::is_lvalue_reference_v<T>) {
            return *reinterpret_cast<std::remove_reference_t<T>*>(static_cast<const UInt8*>(basePtr) + offset);
        } else if constexpr (std::is_pointer_v<T>) {
            return reinterpret_cast<T>(static_cast<const UInt8*>(basePtr) + offset);
        } else {
            static_assert(AlwaysFalse<T>(), "AccessByOffset requires a reference or a pointer type.");
        }
    }

    template <typename To, typename From>
    constexpr To BitCast(const From& value) noexcept {
        static_assert(sizeof(From) == sizeof(To));
        static_assert(std::is_trivially_copyable_v<From>);
        static_assert(std::is_trivially_copyable_v<To>);

#ifdef FL_HAS_CONSTEXPR_BITCAST_STD
        return std::bit_cast<To>(value);
#elif defined(FL_HAS_CONSTEXPR_BITCAST_BUILTIN)
        return __builtin_bit_cast(To, value);
#else
        To result;
        std::memcpy(&result, &value, sizeof(To));

        return result;
#endif
    }

    template <typename T, std::size_t N>
    constexpr std::size_t CountOf(T (&name)[N]) noexcept {
        return N;
    }

    template <typename T>
    constexpr std::size_t CountOf(const T& c) noexcept {
        return c.size();
    }

    template <typename P, typename T>
    constexpr P IntegerToPointer(T ptrAsInt) noexcept {
        static_assert(std::is_pointer_v<P>);

        return BitCast<P>(SafeCast<std::uintptr_t>(ptrAsInt));
    }

    template <typename T, typename P>
    constexpr T PointerToInteger(P* ptr) noexcept {
        return SafeCast<T>(BitCast<std::uintptr_t>(ptr));
    }

    template <typename M, typename T>
    constexpr auto& Retrieve(M& map, const T& key) noexcept {
        auto it = map.find(key);
        FlAssert(it != map.end());
        return it->second;
    }

    template <typename M, typename T>
    constexpr const auto& Retrieve(const M& map, const T& key) noexcept {
        auto it = map.find(key);
        FlAssert(it != map.end());
        return it->second;
    }

    FL_WARNING_PUSH()
    FL_WARNING_MSVC_DISABLE(4702)

    template <typename To, typename From>
    FL_CONSTEXPR20 To SafeCast(From&& value) noexcept {
#if defined(FL_DEBUG)
        if constexpr (std::is_integral_v<To>) {
            if constexpr (std::is_enum_v<From>) {
                return SafeCast<To>(static_cast<std::underlying_type_t<From>>(value));
            } else if constexpr (std::is_floating_point_v<From>) {
                FlAssert(static_cast<long long>(value) == value);
                FlAssert(value <= static_cast<From>(std::numeric_limits<To>::max()));
                FlAssert(value >= static_cast<From>(std::numeric_limits<To>::lowest()));
            } else if constexpr (std::is_integral_v<From>) {
                // Type capable of storing the biggest value between the two types
                using MaxValueType =
                    std::conditional_t<((!std::is_signed_v<From> && std::is_signed_v<To>) ||
                                        (std::is_signed_v<From> == std::is_signed_v<To> && sizeof(From) > sizeof(To))),
                                       From, To>;
                // Type capable of storing the smallest value between the two types
                using MinValueType =
                    std::conditional_t<((std::is_signed_v<From> && !std::is_signed_v<To>) ||
                                        (std::is_signed_v<From> == std::is_signed_v<To> && sizeof(From) > sizeof(To))),
                                       From, To>;

                if constexpr (!std::is_signed_v<To>)
                    FlAssert(value >= 0);

                FlAssert(static_cast<MaxValueType>(value) <=
                         static_cast<MaxValueType>(std::numeric_limits<To>::max()));
                FlAssert(static_cast<MinValueType>(value) >=
                         static_cast<MinValueType>(std::numeric_limits<To>::lowest()));
            }
        } else if constexpr (std::is_enum_v<To>) {
            return static_cast<To>(SafeCast<std::underlying_type_t<To>>(value));
        } else if constexpr (std::is_floating_point_v<To>) {
            if constexpr (std::is_floating_point_v<From>) {
                // Type capable of storing the biggest value between the two types
                using MaxValueType = std::conditional_t<(sizeof(From) > sizeof(To)), From, To>;
                // Type capable of storing the smallest value between the two types
                using MinValueType = std::conditional_t<(sizeof(From) > sizeof(To)), From, To>;

                FlAssert(static_cast<MaxValueType>(value) <=
                         static_cast<MaxValueType>(std::numeric_limits<To>::max()));
                FlAssert(static_cast<MinValueType>(value) >=
                         static_cast<MinValueType>(std::numeric_limits<To>::lowest()));
            }
        } else if constexpr (std::is_reference_v<To>) {
            if constexpr (std::is_reference_v<From>) {
                using BaseFromType = std::remove_reference_t<std::remove_cv_t<From>>;
                using BaseToType = std::remove_reference_t<std::remove_cv_t<To>>;

                if constexpr (!std::is_same_v<BaseFromType, BaseToType> && std::is_base_of_v<From, To> &&
                              std::is_polymorphic_v<From>) {
                    using ToPtr = std::add_pointer_t<std::remove_reference_t<To>>;
                    FlAssert(dynamic_cast<ToPtr>(&value) != nullptr);
                }
            }
        } else if constexpr (std::is_pointer_v<To>) {
            if constexpr (std::is_pointer_v<From>) {
                using BaseFromType = std::remove_pointer_t<std::remove_cv_t<From>>;
                using BaseToType = std::remove_pointer_t<std::remove_cv_t<To>>;

                if constexpr (!std::is_same_v<BaseFromType, BaseToType> && std::is_base_of_v<From, To> &&
                              std::is_polymorphic_v<From>) {
                    FlAssert(!value || dynamic_cast<To>(value) != nullptr);
                }
            }
        }

#endif

        return static_cast<To>(value);
    }

    FL_WARNING_POP()

    template <typename From>
    FL_CONSTEXPR20 auto SafeCaster(From&& value) noexcept {
        return Detail::SafeCaster<decltype(value)>(std::forward<From>(value));
    }

    template <typename T, typename U>
    std::unique_ptr<T> StaticUniquePointerCast(std::unique_ptr<U>&& ptr) noexcept {
        return std::unique_ptr<T>(SafeCast<T*>(ptr.release()));
    }

    template <typename T>
    constexpr auto UnderlyingCast(T value) noexcept -> std::underlying_type_t<T> {
        return static_cast<std::underlying_type_t<T>>(value);
    }
} // namespace Fl
