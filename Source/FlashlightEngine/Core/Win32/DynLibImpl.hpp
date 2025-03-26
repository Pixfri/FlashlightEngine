// Copyright (C) 2025 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#ifndef FL_CORE_WIN32_DYNLIBIMPL_HPP
#define FL_CORE_WIN32_DYNLIBIMPL_HPP

#include <FlashlightEngine/Prerequisites.hpp>
#include <FlashlightEngine/Core/BaseObject.hpp>
#include <FlashlightEngine/Core/DynLib.hpp>
#include <FlashlightEngine/Utility/MovablePtr.hpp>

#include <Windows.h>

#include <filesystem>

namespace Fl::PlatformImpl {
    class FL_API DynLibImpl final : public BaseObject {
    public:
        DynLibImpl() = default;
        ~DynLibImpl() override;

        DynLibImpl(const DynLibImpl&) = delete;
        DynLibImpl(DynLibImpl&&) noexcept = default;

        DynLibFunc GetSymbol(const char* symbol, std::string* errorMessage) const;
        bool Load(const std::filesystem::path& path, std::string* errorMessage);

        DynLibImpl& operator=(const DynLibImpl&) = delete;
        DynLibImpl& operator=(DynLibImpl&&) noexcept = default;
    
    private:
        MovablePtr<std::remove_pointer_t<HMODULE>> m_handle;
    };
}

#endif // FL_CORE_WIN32_DYNLIBIMPL_HPP
