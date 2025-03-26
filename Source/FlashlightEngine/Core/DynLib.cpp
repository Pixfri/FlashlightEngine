// Copyright (C) 2025 Jean "Pixfri" Letessier
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#include <FlashlightEngine/Core/DynLib.hpp>

#include <FlashlightEngine/Utility/Assert.hpp>

#if defined(FL_PLATFORM_WINDOWS)
#   include <FlashlightEngine/Core/Win32/DynLibImpl.hpp>
#elif defined(FL_PLATFORM_POSIX)
#   include <FlashlightEngine/Core/Posix/DynLibImpl.hpp>
#else
#   error Current platform has no implementation for DynLib
#endif

namespace Fl {
    DynLib::DynLib() = default;
    DynLib::~DynLib() = default;

    DynLib::DynLib(DynLib&&) noexcept = default;

    std::string DynLib::GetLastError() const {
        return m_lastError;
    }

    DynLibFunc DynLib::GetSymbol(const char* symbol) const {
        FlAssertMsg(IsLoaded(), "[Core/DynLib] Library is not loaded.");

        return m_impl->GetSymbol(symbol, &m_lastError);
    }

    bool DynLib::IsLoaded() const {
        return m_impl != nullptr;
    }

    bool DynLib::Load(std::filesystem::path libraryPath) {
        Unload();

        if (libraryPath.extension().empty()) {
            libraryPath += FL_DYNLIB_EXTENSION;
        }

        auto impl = std::make_unique<PlatformImpl::DynLibImpl>();
        if (!impl->Load(libraryPath, &m_lastError)) {
            FlAssertMsg(false, "Failed to load library '{}'.", m_lastError);
            return false;
        }

        m_impl = std::move(impl);
        return true;
    }

    void DynLib::Unload() {
        m_impl.reset();
    }

    DynLib& DynLib::operator=(DynLib&&) noexcept = default;
} // namespace Fl

#if defined(FL_PLATFORM_WINDOWS)
#include <FlashlightEngine/Core/AntiWindows.hpp>
#endif
