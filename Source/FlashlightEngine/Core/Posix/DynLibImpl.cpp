// Copyright (C) 2025 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#include <FlashlightEngine/Core/Posix/DynLibImpl.hpp>
#include <FlashlightEngine/Utility/Algorithm.hpp>
#include <FlashlightEngine/Utility/PathUtils.hpp>

#include <dlfcn.h>

#include <cstring>

namespace Fl::PlatformImpl {
    DynLibImpl::~DynLibImpl() {
        if (m_handle) {
            dlclose(m_handle);
        }
    }

    DynLibFunc DynLibImpl::GetSymbol(const char* symbol, std::string* errorMessage) const {
        dlerror(); //< Clear error flag.

        void* ptr = dlsym(m_handle, symbol);
        if (!ptr) {
            *errorMessage = dlerror();
        }

        return BitCast<DynLibFunc>(ptr);
    }

    bool DynLibImpl::Load(const std::filesystem::path& path, std::string* errorMessage) {
        dlerror(); //< Clear error flag.
        m_handle = dlopen(path.c_str(), RTLD_LAZY | RTLD_GLOBAL);

        if (!m_handle) {
            *errorMessage = dlerror();
            return false;
        }

        return true;
    }
}
