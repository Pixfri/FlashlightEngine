// Copyright (C) 2025 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#ifndef FL_CORE_APPLICATIONCOMPONENT_HPP
#define FL_CORE_APPLICATIONCOMPONENT_HPP

#include <FlashlightEngine/Prerequisites.hpp>

#include <memory>

namespace Fl {
    struct FrameTimeInfo;

    class ApplicationComponent;
    using ApplicationComponentPtr = std::unique_ptr<ApplicationComponent>;

    /**
     * @brief ApplicationComponent class. Represents components that can be plugged into the application to add features to it.
     */
    class ApplicationComponent {
    public:
        /**
         * @brief Gets the application component type's id.
         *
         * It uses CRTP to assign a different id to each component type it is called with.
         * This function will be instantiated every time it is called with a different type, incrementing the assigned
         * index.
         * Note that it must be called directly from Fl::ApplicationComponent, and a derived class must be given
         * (ApplicationComponent::GetId<DerivedApplicationComponent>()).
         *
         * @tparam AppComponentType Type of the application component to get the id from.
         * @return The application component's id.
         */
        template <typename AppComponentType>
        static U64 GetId();

        /**
         * @brief Updates the application component.
         * @param timeInfo Time-related frame information.
         * @return Whether the application component is still active or not.
         */
        inline virtual bool Update([[maybe_unused]] const FrameTimeInfo& timeInfo);

        virtual ~ApplicationComponent() = default;

    protected:
		ApplicationComponent() = default;

        ApplicationComponent(const ApplicationComponent&) = default;
        ApplicationComponent(ApplicationComponent&&) noexcept = default;

        ApplicationComponent& operator=(const ApplicationComponent&) = default;
        ApplicationComponent& operator=(ApplicationComponent&&) noexcept = default;

    private:
        static inline U64 s_MaxId = 0;
    };
}

#include <FlashlightEngine/Core/ApplicationComponent.inl>

#endif // FL_CORE_APPLICATIONCOMPONENT_HPP
