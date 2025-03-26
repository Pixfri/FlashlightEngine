// Copyright (C) 2025 Jean "Pixfri" Letessier
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#ifndef FL_DYNLIB_HPP
#define FL_DYNLIB_HPP

#include <FlashlightEngine/Core/BaseObject.hpp>
#include <FlashlightEngine/Prerequisites.hpp>

#include <filesystem>
#include <memory>

#if defined(FL_PLATFORM_WINDOWS)
#define FL_DYNLIB_EXTENSION ".dll"
#elif defined(FL_PLATFORM_LINUX)
#define FL_DYNLIB_EXTENSION ".so"
#elif defined(FL_PLATFORM_MACOS)
#define FL_DYNLIB_EXTENSION ".dylib"
#endif

namespace Fl {
    using DynLibFunc = void (*)(void); //< "Generic" type of pointer to function.

    namespace PlatformImpl {
        class DynLibImpl;
    }

    /**
     * @brief Represents and dynamic library loader.
     */
    class FL_API DynLib final : public BaseObject {
    public:
        DynLib() = default;
        ~DynLib() override = default;

        DynLib(const DynLib&) = delete;
        DynLib(DynLib&&) noexcept = default;

        /**
         * @brief Gets the last system error emitted.
         * @return The last system error emitted when loading.
         */
        std::string GetLastError() const;
        /**
         * @brief Retrieves the symbol with the given name.
         * @param symbol Name of the symbol to retrieve.
         * @return The retrieved symbol.
         */
        DynLibFunc GetSymbol(const char* symbol) const;

        /**
         * @brief Checks whether the library is loaded.
         * @return Whether the library is loaded.
         */
        bool IsLoaded() const;

        /**
         * @brief Loads the library with the given path.
         * @remark Sends a FlError() if the library couldn't be loaded.
         * @param libraryPath Path of the library to load.
         * @return Whether the library was loaded correctly.
         */
        bool Load(std::filesystem::path libraryPath);
        /**
         * @brief Unloads the library.
         */
        void Unload();

        DynLib& operator=(const DynLib&) = delete;
        DynLib& operator=(DynLib&&) noexcept = default;

    private:
        mutable std::string m_lastError;
        std::unique_ptr<PlatformImpl::DynLibImpl> m_impl;
    };
} // namespace Fl

#include <FlashlightEngine/Core/DynLib.inl>

#endif // FL_DYNLIB_HPP

