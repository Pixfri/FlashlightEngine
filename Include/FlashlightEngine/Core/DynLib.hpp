// Copyright (C) 2025 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#ifndef FL_DYNLIB_HPP
#define FL_DYNLIB_HPP

#include <FlashlightEngine/Prerequisites.hpp>
#include <FlashlightEngine/Core/BaseObject.hpp>

#include <FlashlightEngine/Core/FunctionRef.hpp>

#include <filesystem>
#include <memory>

#if defined(FL_PLATFORM_WINDOWS)
#   define FL_DYNLIB_EXTENSION ".dll"
#elif defined(FL_PLATFORM_LINUX)
#   define FL_DYNLIB_EXTENSION ".so"
#elif defined(FL_PLATFORM_MACOS)
#   define FL_DYNLIB_EXTENSION ".dylib"
#endif

namespace Fl {
    class DynLibImpl;

    class FL_API DynLib final : public BaseObject {
    public:
        DynLib() = default;
        ~DynLib() override;

        DynLib(const DynLib&) = delete;
        DynLib(DynLib&&) noexcept = default;

        bool IsLoaded() const;
        bool Load(const std::filesystem::path& path);
        bool Unload();
        void* GetSymbol(const std::string& symbol) const;

        template <typename ReturnValue, typename... Args>
        ReturnValue Invoke(const std::string& functionName, Args&&... args);

        template <typename ReturnValue, typename... Args>
        FunctionRef<ReturnValue(Args...)> GetFunction(const std::string& functionName);

        template <typename T>
        T* GetValue(const std::string& valueName);

        DynLib& operator=(const DynLib&) = delete;
        DynLib& operator=(DynLib&&) noexcept = default;
    
    private:
        struct ImplDeleter {
            void operator()(void* impl) const;
        };
        std::unique_ptr<void, ImplDeleter> m_impl;
        mutable std::string m_lastError;
    };
}

#include <FlashlightEngine/Core/DynLib.inl>

#endif // FL_DYNLIB_HPP
