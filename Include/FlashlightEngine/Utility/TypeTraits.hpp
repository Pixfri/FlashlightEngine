// Copyright (C) 2025 Jean "Pixfri" Letessier
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#ifndef FL_UTILITY_TYPETRAITS_HPP
#define FL_UTILITY_TYPETRAITS_HPP

#include <cstddef>
#include <type_traits>

namespace Fl {
    /************************************************************************/

    template <typename T>
    struct AlwaysFalse : std::false_type {};

    template <auto T>
    struct AlwaysFalseValue : std::false_type {};

    /************************************************************************/

    template <typename T>
    struct LazyVoid {
        using type = void;
    };

    template <typename T>
    using LazyVoid_t = typename LazyVoid<T>::type;

    /************************************************************************/

    template <typename E, typename = void>
    struct EnumValueCount {
        static_assert(AlwaysFalse<E>(), "Enum has no Count nor Max field.");
    };

    template <typename E>
    struct EnumValueCount<E, LazyVoid_t<decltype(E::Count)>> {
        static_assert(std::is_enum_v<E>, "Type must be an enumeration.");

        static constexpr std::size_t value = static_cast<std::size_t>(E::Count);
    };


    template <typename E>
    struct EnumValueCount<E, LazyVoid_t<decltype(E::Max)>> {
        static_assert(std::is_enum_v<E>, "Type must be an enumeration.");

        static constexpr std::size_t value = static_cast<std::size_t>(E::Max) + 1;
    };

    template <typename E>
    constexpr std::size_t EnumValueCount_v = EnumValueCount<E>::value;

    /************************************************************************/

    namespace Detail {
        template <typename T>
        struct FunctionPtrType;

        template <typename Ret, typename... Args>
        struct FunctionPtrType<Ret(Args...)> {
            using type = Ret (*)(Args...);
        };
    } // namespace Detail

    template <typename T>
    using FunctionPtr = typename Detail::FunctionPtrType<T>::type;

    /************************************************************************/

    namespace Detail {
        template <typename T>
        struct IsCompleteHelper {
            // SFINAE: sizeof in an incomplete type is an error, but since there's another specialization it won't
            // result in a compilation error
            template <typename U>
            static auto test(U*) -> std::bool_constant<sizeof(U) == sizeof(U)>;

            // Less specialized overload.
            static auto test(...) -> std::false_type;

            using type = decltype(test(static_cast<T*>(nullptr)));
        };
    } // namespace Detail

    template <typename T>
    struct IsComplete : Detail::IsCompleteHelper<T>::type {};

    template <typename T>
    constexpr bool IsComplete_v = IsComplete<T>::value;

    /************************************************************************/

    template <typename T, typename... Args>
    struct IsFirstType : std::false_type {};

    template <typename T, typename... Args>
    struct IsFirstType<T, T, Args...> : std::true_type {};

    template <typename T, typename... Args>
    constexpr bool IsFirstType_v = IsFirstType<T, Args...>::value;

    /************************************************************************/

    // Helper for std::visit
    template <typename... Types>
    struct Overloaded : Types... {
        using Types::operator()...;
    };

    template <typename... Types>
    Overloaded(Types...) -> Overloaded<Types...>;

    /************************************************************************/

    template <typename T>
    struct PointedType {
        static_assert(AlwaysFalse<T>(), "Not a pointer");
    };

    template <typename T>
    struct PointedType<T*> {
        using type = T;
    };

    template <typename T>
    struct PointedType<T* const> {
        using type = T;
    };

    template <typename T>
    struct PointedType<T* volatile> {
        using type = T;
    };

    template <typename T>
    struct PointedType<T* const volatile> {
        using type = T;
    };

    /************************************************************************/

    template <typename T>
    using Pointer = T*;

    /************************************************************************/

    // same as std::remove_cvref but before C++20
    template <typename T>
    struct RemoveCvRef {
        using type = std::remove_cv_t<std::remove_reference_t<T>>;
    };

    template <typename T>
    using RemoveCvRef_t = typename RemoveCvRef<T>::type;

    /************************************************************************/

    // Detects if the first parameter of a parameter pack is a type, which could be called instead of the
    // copy/move constructor/assignation operator
    template<typename T, typename... Args>
    struct PreventHiddenCopyMoveImpl {
        using type = void;
    };

    template <typename T, typename T2>
    struct PreventHiddenCopyMoveImpl<T, T2> : std::enable_if_t<!std::is_same_v<T, RemoveCvRef_t<T2>>> {};

    template<typename T, typename... Args>
    using PreventHiddenCopyMove = typename PreventHiddenCopyMoveImpl<T, Args...>::type;
} // namespace Fl

#endif // FL_UTILITY_TYPETRAITS_HPP
