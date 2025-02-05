// Copyright (C) 2025 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#ifndef FL_CORE_WORLD_HPP
#define FL_CORE_WORLD_HPP

#include <FlashlightEngine/Prerequisites.hpp>

#include <FlashlightEngine/Core/Entity.hpp>
#include <FlashlightEngine/Core/System.hpp>
#include <FlashlightEngine/DataStructures/Bitset.hpp>

#include <stdexcept>

namespace Fl {
    struct FrameTimeInfo;

    class World;
    using WorldPtr = std::unique_ptr<World>;

    class World {
    public:
        World() = default;
        inline explicit World(U64 entityCount);
        inline ~World();

        World(const World&) = delete;
        World(World&&) noexcept = default;

        [[nodiscard]] inline const std::vector<SystemPtr>& GetSystems() const;
        [[nodiscard]] inline const std::vector<EntityPtr>& GetEntities() const;

        /**
         * @brief Adds a system to the world.
         * @tparam SystemType Type of the system to add.
         * @tparam Args Types of arguments to forward to the system.
         * @param args Arguments to forward to the system.
         * @return Reference to the added system.
         */
        template <typename SystemType, typename... Args>
        SystemType& AddSystem(Args&&... args);

        /**
         * @brief Checks if a given system is attached to the world.
         * @tparam SystemType Type of the system to check.
         * @return Whether the given system is attached to the world.
         */
        template <typename SystemType>
        [[nodiscard]] bool HasSystem() const;

        /**
         * @brief Retrieves the system with the given type.
         * @tparam SystemType Type of the system to retrieve.
         * @return A constant reference to the given system.
         * @throws std::runtime_error If the given system isn't attached to the world.
         */
        template <typename SystemType>
        const SystemType& GetSystem();

        /**
         * @brief Retrieves the system with the given type.
         * @tparam SystemType Type of the system to retrieve.
         * @return A reference to the given system.
         * @throws std::runtime_error If the given system isn't attached to the world.
         */
        template <typename SystemType>
        SystemType& GetSystem();

        /**
         * @brief Removes the given system from the world.
         * @tparam SystemType Type of the system to remove.
         */
        template <typename SystemType>
        void RemoveSystem();

        /**
         * @brief Adds an entity into the world with no components.
         * @param enabled Whether the entity should be enabled or not.
         * @return A reference to the new entity.
         */
        Entity& AddEntity(bool enabled = true);

        /**
         * @brief Adds an entity into the world with a given component. This entity is automatically enabled.
         * @tparam ComponentType The type of the component to add.
         * @tparam Args Types of arguments to forward to the component.
         * @param args Arguments to forward to the component.
         * @return A reference to the new entity.
         */
        template <typename ComponentType, typename... Args>
        Entity& AddEntityWithComponent(Args&&... args);

        /**
         * @brief Adds an entity into the world with a list of components.
         * @tparam ComponentTypes Types of the components to add.
         * @param enabled Whether the entity should be enabled or not.
         * @return A reference to the new entity.
         */
        template <typename... ComponentTypes>
        Entity& AddEntityWithComponents(bool enabled = true);

        /**
         * @brief Retrieves entities in the world with the given components.
         * @tparam ComponentTypes Types of components the entities must have to be retrieved.
         * @return A std::vector of pointers to the entities with the requested components.
         */
        template <typename... ComponentTypes>
        [[nodiscard]] std::vector<Entity*> GetEntitiesWithComponents() const;

        /**
         * @brief Removes an entity from the world.
         * @param entity Entity to remove.
         * @throws std::invalid_argument If the entity isn't owned by this world.
         */
        void RemoveEntity(const Entity& entity);

        /**
         * @brief Updates the world and all the systems it contains.
         * @param timeInfo Time-related frame information.
         * @return Whether the world still has running systems or not.
         */
        bool Update(const FrameTimeInfo& timeInfo);

        /**
         * @brief Refreshes the world to optimize the entities and link/unlink entities from systems if required.
         */
        void Refresh();

        /**
         * @brief Destroys the world, releasing all of its entities and systems.
         */
        void Destroy();

        World& operator=(const World&) = delete;
        World& operator=(World&&) noexcept = default;

    private:
        /**
         * @brief Sorts entities so that the disabled ones are packed to the end of the list.
         */
        void SortEntities();

        std::vector<SystemPtr> m_Systems{};
        Bitset m_ActiveSystems{};

        std::vector<EntityPtr> m_Entities{};
        U64 m_ActiveEntityCount = 0;
        U64 m_MaxEntityIndex = 0;
    };
}

#include <FlashlightEngine/Core/World.inl>

#endif // FL_CORE_WORLD_HPP
