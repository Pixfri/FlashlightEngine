// Copyright (C) 2025 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#ifndef FL_CORE_POSIX_DYNLIBIMPL_HPP
#define FL_CORE_POSIX_DYNLIBIMPL_HPP

#include <FlashlightEngine/Prerequisites.hpp>
#include <FlashlightEngine/Core/BaseObject.hpp>
#include <FlashlightEngine/Core/DynLib.hpp>
#include <FlashlightEngine/Utility/MovablePtr.hpp>

#include <filesystem>
#include <string>

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
        MovablePtr<void> m_handle;
    };
}

#endif // FL_CORE_POSIX_DYNLIBIMPL_HPP
