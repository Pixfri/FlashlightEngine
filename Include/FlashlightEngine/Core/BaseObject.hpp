// Copyright (C) 2025 Jean "Pixfri" Letessier
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#ifndef FL_CORE_BASEOBJECT_HPP
#define FL_CORE_BASEOBJECT_HPP

#include <FlashlightEngine/Prerequisites.hpp>

#include <string>

namespace Fl {
    /**
     * @brief Base class for all classes in the engine.
     *
     * This class is used mainly to get the ID of classes in the context of serialization/deserialization.
     */
    class FL_API BaseObject {
    public:
        virtual ~BaseObject() = default;

        struct ClassInfo {
            std::string name;
            UInt64 id;
        };

        /**
         * @brief Gets the info of the given class.
         * This uses CRTP to assign a different ID to each class type it is called with.
         * This function will be instantiated every time it is called with a different type, incrementing the assigned
         * index.
         * Some compiler trickery is also done to get the class's name, because compilers couldn't agree on the
         * formatting of the string returned by typeid().name().
         * @note Must be called directly from Fl::BaseObject, and a derived class must be given
         *       (BaseObject::GetInfo<DerivedClass>()).
         * @tparam T The class to get the ID of.
         * @return Given class' ID.
         */
        template <class T>
            requires(std::is_base_of_v<BaseObject, T>, !std::is_same_v<T, BaseObject>)
        static ClassInfo GetInfo() noexcept;

    protected:
        BaseObject() = default;

        BaseObject(const BaseObject&) = default;
        BaseObject(BaseObject&&) noexcept = default;

        BaseObject& operator=(const BaseObject&) = default;
        BaseObject& operator=(BaseObject&&) noexcept = default;

    private:
#if defined(FL_COMPILER_CLANG) || defined(FL_COMPILER_GCC)
        /**
         * @brief De-mangles the name returned by GCC and clang when calling typeid(type).name().
         * @param mangled Mangled class name.
         * @return A demangled version of the class's name.
         */
        static inline std::string DemangleClassName(const char* mangled);
#endif

        static inline UInt64 s_maxId = 0;
    };
} // namespace Fl

#include <FlashlightEngine/Core/BaseObject.inl>

#endif // FL_CORE_BASEOBJECT_HPP
