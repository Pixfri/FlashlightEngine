// Copyright (C) 2025 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#include <FlashlightEngine/Core/DynLib.hpp>

#include <stdexcept>

namespace Fl {
    template <typename ReturnValue, typename... Args>
    ReturnValue DynLib::Invoke(const std::string& functionName, Args&&... args) {
        auto func = GetFunction<ReturnValue, Args...>(functionName);
        if (!func) {
            FlAssertAlwaysFalse("[Core/DynLib] Invalid function pointer.");
            throw std::runtime_error("[Core/DynLib] Invalid function pointer.");
        }

        return func(std::forward<Args>(args)...);
    }

    template <typename ReturnValue, typename... Args>
    FunctionRef<ReturnValue(Args...)> DynLib::GetFunction(const std::string& functionName) {
        void* symbol = GetSymbol(functionName);
        if (!symbol) {
            FlAssertAlwaysFalse("[Core/DynLib] Invalid symbol pointer '{}'.", functionName);
            return FunctionRef<ReturnValue(Args...)>();
        }

        using Func = ReturnValue(*)(Args...);
        return FunctionRef<ReturnValue(Args...)>(reinterpret_cast<Func>(symbol));
    }

    template <typename T>
    T* DynLib::GetValue(const std::string& valueName) {
        void* symbol = GetSymbol(valueName);
        if (!symbol) {
            FlAssertAlwaysFalse("[Core/DynLib] Invalid symbol pointer '{}'.", valueName);
            return nullptr;
        }

        return static_cast<T*>(symbol);
    }
}
