// Copyright (C) 2025 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#ifndef FL_CORE_COMPONENT_HPP
#define FL_CORE_COMPONENT_HPP

#include <FlashlightEngine/Prerequisites.hpp>

#include <memory>

namespace Fl {
    class Component;
    using ComponentPtr = std::unique_ptr<Component>;

    /**
     * @brief A base class components that can be attached to entities inherit from.
     */
    class Component {
    public:
        /**
         * @brief Gets the component type's id.
         *
         * It uses CRTP to assign a different id to each component type it is called with.
         * This function will be instantiated every time it is called with a different type, incrementing the assigned
         * index.
         * Note that it must be called directly from Fl::Component, and a derived class must be given
         * (Component::GetId<DerivedComponent>()).
         *
         * @tparam ComponentType Type of the component to get the id from.
         * @return The component's id.
         */
        template <typename ComponentType>
        static U64 GetId();

        virtual ~Component() = default;

    protected:
        Component() = default;

        Component(const Component&) = default;
        Component(Component&&) noexcept = default;

        Component& operator=(const Component&) = default;
        Component& operator=(Component&&) noexcept = default;

    private:
        static inline U64 s_MaxId = 0;
    };
}

#include <FlashlightEngine/Core/Component.inl>

#endif // FL_CORE_COMPONENT_HPP
