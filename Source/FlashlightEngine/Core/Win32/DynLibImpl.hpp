// Copyright (C) 2025 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#ifndef FL_CORE_WIN32_DYNLIBIMPL_HPP
#define FL_CORE_WIN32_DYNLIBIMPL_HPP

#include <FlashlightEngine/Prerequisites.hpp>
#include <FlashlightEngine/Core/BaseObject.hpp>

#include <Windows.h>

#include <filesystem>
#include <string>
#include <string_view>

namespace Fl {
    class FL_API DynLibImpl : public BaseObject {
    public:
        DynLibImpl() = default;
        DynLibImpl(const DynLibImpl&) = delete;

        bool Load(const std::filesystem::path& filePath, std::string* error = nullptr);
        bool Unload(std::string* error = nullptr);
        void* GetSymbol(const std::string& symbol, std::string* error = nullptr) const;
    
    private:
        HMODULE m_module = {};
    };
}

#endif // FL_CORE_WIN32_DYNLIBIMPL_HPP
