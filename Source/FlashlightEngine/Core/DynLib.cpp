// Copyright (C) 2025 Jean "Pixfri" Letessier
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#include <FlashlightEngine/Core/DynLib.hpp>

#include <FlashlightEngine/Core/Assert.hpp>

#if defined(FL_PLATFORM_WINDOWS)
#   include <FlashlightEngine/Core/Win32/DynLibImpl.hpp>
#elif defined(FL_PLATFORM_POSIX)
#   include <FlashlightEngine/Core/Posix/DynLibImpl.hpp>
#else
#   error Current platform has no implementation for DynLib
#endif

#define FL_TO_DYNLIB_IMPL static_cast<DynLibImpl*>(m_impl.get())

namespace Fl {
    DynLib::~DynLib() {
        if (IsLoaded()) {
            FL_TO_DYNLIB_IMPL->Unload();
        }

        m_impl = nullptr;
    }

    bool DynLib::IsLoaded() const {
        return m_impl != nullptr;
    }

    bool DynLib::Load(const std::filesystem::path& path) {
        FlAssert(m_impl, "[Core/DynLib] A DynLib is already loaded, please call Fl::DynLib::Unload before.");
        m_impl = std::unique_ptr<void, ImplDeleter>(new DynLibImpl, ImplDeleter());
        bool result = false;
        if (path.extension() != FL_DYNLIB_EXTENSION) {
            result = FL_TO_DYNLIB_IMPL->Load(path.string() + FL_DYNLIB_EXTENSION, &m_lastError);
        } else {
            result = FL_TO_DYNLIB_IMPL->Load(path, &m_lastError);
        }

        if (result) {
            return true;
        }

        m_impl = nullptr;
        return false;
    }

    bool DynLib::Unload() {
        if (!m_impl) {
            return false;
        }

        const bool result = FL_TO_DYNLIB_IMPL->Unload(&m_lastError);

        m_impl = nullptr;
        return result;
    }

    void* DynLib::GetSymbol(const std::string& symbol) const {
        if (!m_impl) {
            return nullptr;
        }

        void* result = FL_TO_DYNLIB_IMPL->GetSymbol(symbol, &m_lastError);
        return result;
    }

    void DynLib::ImplDeleter::operator()(void* impl) const {
        if (!impl) {
            return;
        }

        std::default_delete<DynLibImpl>()(static_cast<DynLibImpl*>(impl));
    }
} // namespace Fl
