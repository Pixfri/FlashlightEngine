// Copyright (C) 2025 Jean "Pixfri" Letessier
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#include <FlashlightEngine/Core/Win32/DynLibImpl.hpp>

#include <FlashlightEngine/Core/SystemError.hpp>
#include <FlashlightEngine/Core/Win32/Win32Utils.hpp>

namespace Fl::PlatformImpl {
    DynLibImpl::~DynLibImpl() {
        if (m_handle) {
            FreeLibrary(m_handle);
        }
    }

    DynLibFunc DynLibImpl::GetSymbol(const char* symbol, std::string* errorMessage) const {
        const auto func = reinterpret_cast<DynLibFunc>(::GetProcAddress(m_handle, symbol));

        if (!func) {
            *errorMessage = SystemError::GetLastSystemError();
        }

        return func;
    }

    bool DynLibImpl::Load(const std::filesystem::path& path, std::string* errorMessage) {
        m_handle = LoadLibraryExW(PathToWideTemp(path).data(), nullptr, (path.is_absolute()) ? LOAD_WITH_ALTERED_SEARCH_PATH : 0);

        if (m_handle) {
            return true;
        }

        *errorMessage = SystemError::GetLastSystemError();
        return false;
    }
} // namespace Fl::PlatformImpl
