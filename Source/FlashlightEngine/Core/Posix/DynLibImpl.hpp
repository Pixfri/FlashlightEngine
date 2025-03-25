// Copyright (C) 2025 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#ifndef FL_CORE_POSIX_DYNLIBIMPL_HPP
#define FL_CORE_POSIX_DYNLIBIMPL_HPP

#include <FlashlightEngine/Prerequisites.hpp>
#include <FlashlightEngine/Core/BaseObject.hpp>

#include <dlfcn.h>

#include <filesystem>
#include <string>

namespace Fl {
    class FL_API DynLibImpl : public BaseObject {
    public:
        DynLibImpl() = default;
        DynLibImpl(const DynLibImpl&) = delete;

        bool Load(const std::filesystem::path& filePath, std::string* error = nullptr);
        bool Unload(std::string* error = nullptr);
        void* GetSymbol(const std::string& symbol, std::string* error = nullptr) const;
    
    private:
        void* m_module = {};
    };
}

#endif // FL_CORE_POSIX_DYNLIBIMPL_HPP
