// Copyright (C) 2025 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#ifndef FL_CORE_BASEOBJECT_HPP
#define FL_CORE_BASEOBJECT_HPP

#include <FlashlightEngine/Prerequisites.hpp>

namespace Fl {
    /**
     * @brief Base class for all classes in the engine.
     *
     * This class is used mainly to get the ID of classes in the context of serialization/deserialization.
     */
    class FL_API BaseObject {
    public:
        virtual ~BaseObject() = default;

        /**
         * @brief Gets the ID of the given class.
         * This uses CRTP to assign a different ID to each class type it is called with.
         * This function will be instantiated every time it is called with a different type, incrementing the assigned
         * index.
         * @note Must be called directly from Fl::BaseObject, and a derived class must be given
         *       (BaseObject::GetId<DerivedClass>()).
         * @tparam T The class to get the ID of.
         * @return Given class' ID.
         */
        template <class T>
        static UInt64 GetId() noexcept;

    protected:
        BaseObject() = default;

        BaseObject(const BaseObject&) = default;
        BaseObject(BaseObject&&) noexcept = default;

        BaseObject& operator=(const BaseObject&) = default;
        BaseObject& operator=(BaseObject&&) noexcept = default;
    
    private:
        static inline UInt64 s_maxId = 0;
    };
}

#include <FlashlightEngine/Core/BaseObject.inl>

#endif // FL_CORE_BASEOBJECT_HPP
