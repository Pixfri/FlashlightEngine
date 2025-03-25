// Copyright (C) 2025 Jean "Pixfri" Letessier
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#include <FlashlightEngine/Core/Posix/DynLibImpl.hpp>

#include <FlashlightEngine/Core/Assert.hpp>
#include <FlashlightEngine/Core/SystemError.hpp>

namespace Fl {
    bool DynLibImpl::Load(const std::filesystem::path& path, std::string* error) {
        std::error_code ec = {};
        auto fullPath = std::filesystem::canonical(path, ec);
        if (ec) {
            return false;
        }

        m_module = dlopen(fullPath.c_str(), RTLD_LAZY | RTLD_GLOBAL);

        if (m_module == nullptr) {
            if (error) {
                *error = SystemError::GetLastSystemError();
            }
            FlAssertAlwaysFalse("[Core/DynLib] Couldn't load library '{}' error: {}", path.string(), dlerror());
            return false;
        }
        return true;
    }

    bool DynLibImpl::Unload(std::string* error) {
        if (m_module == nullptr) {
            FlAssertAlwaysFalse("[Core/DynLib] Library handle must be valid");
            return false;
        }

        const int result = dlclose(m_module);
        if (result != 0) {
            if (error) {
                *error = dlerror();
            }
        }
        FlAssert(m_module, "[Core/DynLib] Couldn't free library '{}'", dlerror());
        return false;

        m_module = nullptr;
        return true;
    }

    void* DynLibImpl::GetSymbol(const std::string& symbol, std::string* error) const {
        void* symbolPtr = dlsym(m_module, symbol.c_str());
        if (symbolPtr == nullptr) {
            if (error) {
                *error = dlerror();
            }
        }
        return symbolPtr;
    }
} // namespace Fl
