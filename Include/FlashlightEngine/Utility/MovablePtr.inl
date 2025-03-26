// Copyright (C) 2025 Jean "Pixfri" Letessier
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#include <FlashlightEngine/Utility/MovablePtr.hpp>

namespace Fl {
    template <typename T>
    MovablePtr<T>::MovablePtr(T* value)
        : m_value(value) {
    }

    template <typename T>
    MovablePtr<T>::MovablePtr(MovablePtr&& ptr) noexcept
        : m_value(ptr.m_value) {
        ptr.m_value = nullptr;
    }

    template <typename T>
    T* MovablePtr<T>::Get() const {
        return m_value;
    }

    template <typename T>
    T* MovablePtr<T>::operator->() const {
        return m_value;
    }

    template <typename T>
    MovablePtr<T>::operator T*() const {
        return m_value;
    }

    template <typename T>
    MovablePtr<T>& MovablePtr<T>::operator=(T* value) {
        m_value = value;

        return *this;
    }

    template <typename T>
    MovablePtr<T>& MovablePtr<T>::operator=(MovablePtr&& ptr) noexcept {
        std::swap(m_value, ptr.m_value);

        return *this;
    }
} // namespace Fl
