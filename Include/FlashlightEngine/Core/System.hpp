// Copyright (C) 2025 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#ifndef FL_CORE_SYSTEM_HPP
#define FL_CORE_SYSTEM_HPP

#include <FlashlightEngine/Prerequisites.hpp>

#include <FlashlightEngine/Core/Entity.hpp>
#include <FlashlightEngine/DataStructures/Bitset.hpp>

#include <memory>

namespace Fl {
    struct FrameTimeInfo;

    class System;
    using SystemPtr = std::unique_ptr<System>;

    /**
     * @brief A base class systems that can be attached to worlds inherit from.
     */
    class System {
        friend class World;

    public:
        virtual ~System() = default;

        System(const System&) = delete;
        System(System&&) noexcept = delete;

        /**
         * @brief Fetches which components should be accepted by the system.
         * @return A bitset containing which components should be accepted.
         */
        [[nodiscard]] inline const Bitset& GetAcceptedComponents() const;

        /**
         * @brief Gets the system type's id.
         *
         * It uses CRTP to assign a different id to each system type it is called with.
         * This function will be instantiated every time it is called with a different type, incrementing the assigned
         * index.
         * Note that it must be called directly from Fl::System, and a derived class must be given
         * (System::GetId<DerivedSystem>()).
         *
         * @tparam SystemType Type of the system to get the id from.
         * @return The system's id.
         */
        template <typename SystemType>
        static U64 GetId();

        /**
         * @brief Checks if the system contains the given entity.
         * @param entity Entity to be checked.
         * @return Whether the system contains the entity or not.
         */
        [[nodiscard]] bool ContainsEntity(const Entity& entity) const noexcept;

        /**
         * @brief Updates the system.
         * @param timeInfo Time-related frame information.
         * @return Whether the system is still active or not.
         */
        inline virtual bool Update([[maybe_unused]] const FrameTimeInfo& timeInfo);

        /**
         * @brief Destroys the system.
         */
        inline virtual void Destroy();

        System& operator=(const System&) = delete;
        System& operator=(System&&) noexcept = delete;

    protected:
        System() = default;

        /**
         * @brief Set the given components to be accepted by the system.
         * @tparam ComponentTypes Types of the components to accept.
         */
        template <typename... ComponentTypes>
        void RegisterComponents();

        /**
         * @brief Removes the given components from the list of components to be accepted by the system.
         * @tparam ComponentTypes Types of the components to accept.
         */
        template <typename... ComponentTypes>
        void UnregisterComponents();

        /**
         * @brief Links an entity to the system.
         * @param entity Entity to be linked.
         */
        virtual void LinkEntity(const EntityPtr& entity);

        /**
         * @brief Unlinks an entity from the system.
         * @param entity Entity to be unlinked.
         */
        virtual void UnlinkEntity(const EntityPtr& entity);

        std::vector<Entity*> m_Entities{};
        Bitset m_AcceptedComponents{};

    private:
        static inline U64 s_MaxId = 0;
    };
}

#include <FlashlightEngine/Core/System.inl>

#endif // FL_CORE_SYSTEM_HPP
