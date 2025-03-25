// Copyright (C) 2025 Jean "Pixfri" Letessier
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#include <FlashlightEngine/Core/Win32/DynLibImpl.hpp>

#include <FlashlightEngine/Core/Assert.hpp>
#include <FlashlightEngine/Core/SystemError.hpp>

namespace Fl {
    bool DynLibImpl::Load(const std::filesystem::path& filePath, std::string* error) {
        m_module = LoadLibraryW(filePath.c_str());
        if (m_module == nullptr) {
            if (error) {
                *error = SystemError::GetLastSystemError();
            }
            FlAssertAlwaysFalse("[Core/DynLib] Couldn't load library '{}'. Error: {}", filePath.string(),
                                SystemError::GetLastSystemError());

            return false;
        }

        return true;
    }

    bool DynLibImpl::Unload(std::string* error) {
        if (m_module == nullptr) {
            return true;
        }

        if (!FreeLibrary(m_module)) {
            if (error) {
                *error = SystemError::GetLastSystemError();
            }
            FlAssert(m_module, "[Core/DynLib] Couldn't free library. Error: {}", SystemError::GetLastSystemError());
            return false;
        }

        m_module = nullptr;
        return true;
    }

    void* DynLibImpl::GetSymbol(const std::string& symbol, std::string* error) const {
        void* symbolPtr = GetProcAddress(m_module, symbol.c_str());
        if (symbolPtr == nullptr) {
            if (error) {
                *error = SystemError::GetLastSystemError();
            }
        }

        return symbolPtr;
    }
} // namespace Fl
