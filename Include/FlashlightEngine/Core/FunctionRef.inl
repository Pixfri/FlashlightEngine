// Copyright (C) 2025 Jean "Pixfri" Letessier
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#include <FlashlightEngine/Core/FunctionRef.hpp>

#include <FlashlightEngine/Core/Assert.hpp>

#include <functional>

namespace Fl {
    template <typename ReturnValue, typename... Args>
    template <typename Functor>
        requires(std::is_invocable_r_v<ReturnValue, std::decay_t<Functor>, Args...>)
    FunctionRef<ReturnValue(Args...)>::FunctionRef(Functor&& func) :
        m_functionPointer(reinterpret_cast<void*>(std::addressof(func))) {
        m_callback = [](void* function, Args... args) -> ReturnValue {
            return std::invoke(*reinterpret_cast<decltype(std::addressof(func))>(function), std::forward<Args>(args)...);
        };
    }

    template <typename ReturnValue, typename... Args>
    template <typename Functor>
        requires(std::is_invocable_r_v<ReturnValue, std::decay_t<Functor>, Args...>)
    FunctionRef<ReturnValue(Args...)>& FunctionRef<ReturnValue(Args...)>::operator=(Functor&& f) {
        m_functionPointer = reinterpret_cast<void*>(std::addressof(f));
        m_callback = [](void* function, Args... args) -> ReturnValue {
            return std::invoke(*reinterpret_cast<decltype(std::addressof(f))>(function), std::forward<Args>(args)...);
        };

        return *this;
    }

    template <typename ReturnValue, typename... Args>
    template <typename... FunctorArgs>
        requires(std::is_invocable_r_v<ReturnValue, ReturnValue(Args...), FunctorArgs...>)
    ReturnValue FunctionRef<ReturnValue(Args...)>::operator()(FunctorArgs&&... args) const {
        if (this->operator bool() == false) {
            FlAssertAlwaysFalse("[Error/Core] Invalid function pointer.");
            throw std::bad_function_call();
        }

        return m_callback(m_functionPointer, std::forward<FunctorArgs>(args)...);
    }

    template <typename ReturnValue, typename... Args>
    FunctionRef<ReturnValue(Args...)>::operator bool() const noexcept {
        return m_functionPointer != nullptr;
    }
} // namespace Fl
