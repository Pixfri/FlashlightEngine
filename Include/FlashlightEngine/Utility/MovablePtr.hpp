// Copyright (C) 2025 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#ifndef FL_UTILITY_MOVABLEPTR_HPP
#define FL_UTILITY_MOVABLEPTR_HPP

#include <FlashlightEngine/Prerequisites.hpp>
#include <FlashlightEngine/Core/BaseObject.hpp>

namespace Fl {
    /**
     * @brief Wraps a raw (non-proprietary) pointer to allow it to be moved implicitly.
     * @tparam T Type to point to.
     */
    template <typename T>
    class FL_API MovablePtr : public BaseObject {
    public:
        MovablePtr(T* value = nullptr);
        ~MovablePtr() override = default;

        MovablePtr(const MovablePtr&) = default;
        MovablePtr(MovablePtr&& ptr) noexcept;

        T* Get() const;

        T* operator->() const;

        operator T*() const;

        MovablePtr& operator=(T* value);
        MovablePtr& operator=(const MovablePtr&) = default;
        MovablePtr& operator=(MovablePtr&& ptr) noexcept;
    
    private:
        T* m_value;
    };
}

#include <FlashlightEngine/Utility/MovablePtr.inl>

#endif // FL_UTILITY_MOVABLEPTR_HPP
