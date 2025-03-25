// Copyright (C) 2025 Jean "Pixfri" Letessier
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#ifndef FL_FUNCTIONREF_HPP
#define FL_FUNCTIONREF_HPP

#include <FlashlightEngine/Core/BaseObject.hpp>
#include <FlashlightEngine/Prerequisites.hpp>

#include <type_traits>

namespace Fl {
    template <typename T>
    class FunctionRef;

    template <typename ReturnValue, typename... Args>
    class FL_API FunctionRef<ReturnValue(Args...)> final : public BaseObject {
    public:
        using Signature = ReturnValue(Args...);

        FunctionRef() = default;
        template <typename Functor>
            requires(std::is_invocable_r_v<ReturnValue, std::decay_t<Functor>, Args...>)
        FunctionRef(Functor&& func);

        ~FunctionRef() override = default;

        FunctionRef(const FunctionRef&) = default;
        FunctionRef(FunctionRef&&) noexcept = default;

        explicit operator bool() const noexcept;

        template <typename Functor>
            requires(std::is_invocable_r_v<ReturnValue, std::decay_t<Functor>, Args...>)
        FunctionRef& operator=(Functor&& f);

        template <typename... FunctorArgs>
            requires(std::is_invocable_r_v<ReturnValue, ReturnValue(Args...), FunctorArgs...>)
        ReturnValue operator()(FunctorArgs&&... args) const;

        FunctionRef& operator=(const FunctionRef&) = default;
        FunctionRef& operator=(FunctionRef&&) noexcept = default;

    private:
        using Callback = ReturnValue(*)(void*, Args...);
        Callback m_callback;
        void* m_functionPointer = nullptr;
    };

    // Deduction guide.
    template <typename ReturnValue, typename... Args>
    FunctionRef(ReturnValue(*)(Args...)) -> FunctionRef<ReturnValue(Args...)>;
} // namespace Fl

#include <FlashlightEngine/Core/FunctionRef.inl>

#endif // FL_FUNCTIONREF_HPP
