// Copyright (C) 2025 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

namespace Fl {
    inline World::World(const U64 entityCount) {
        m_Entities.reserve(entityCount);
    }

    inline World::~World() {
        Destroy();
    }

    inline const std::vector<SystemPtr>& World::GetSystems() const {
        return m_Systems;
    }

    inline const std::vector<EntityPtr>& World::GetEntities() const {
        return m_Entities;
    }

    template <typename SystemType, typename... Args>
    SystemType& World::AddSystem(Args&&... args) {
        static_assert(std::is_base_of_v<System, SystemType>,
                      "[Error] The system must be derived from Fl::System.");

        const U64 systemId = System::GetId<SystemType>();

        if (systemId >= m_Systems.size()) {
            m_Systems.resize(systemId + 1);
        }

        m_Systems[systemId] = std::make_unique<SystemType>(std::forward<Args>(args)...);
        m_ActiveSystems.SetBit(systemId);

        return static_cast<SystemType&>(*m_Systems[systemId]);
    }

    template <typename SystemType>
    bool World::HasSystem() const {
        static_assert(std::is_base_of_v<System, SystemType>,
                              "[Error] The system must be derived from Fl::System.");

        const U64 systemId = System::GetId<SystemType>();

        return ((systemId < m_Systems.size()) && m_Systems[systemId]);
    }

    template <typename SystemType>
    const SystemType& World::GetSystem() {
        static_assert(std::is_base_of_v<System, SystemType>,
                      "[Error] The system must be derived from Fl::System.");

        if (HasSystem<SystemType>()) {
            return static_cast<const SystemType&>(*m_Systems[System::GetId<SystemType>()]);
        }

        throw std::runtime_error("[Error] No system available for the given type.");
    }

    template <typename SystemType>
    SystemType& World::GetSystem() {
        return const_cast<SystemType&>(static_cast<const World*>(this)->GetSystem<SystemType>());
    }

    template <typename SystemType>
    void World::RemoveSystem() {
        static_assert(std::is_base_of_v<System, SystemType>,
                      "[Error] The system must be derived from Fl::System.");

        if (HasSystem<SystemType>()) {
            m_Systems[System::GetId<SystemType>()].reset();
        }
    }

    template <typename ComponentType, typename... Args>
    Entity& World::AddEntityWithComponent(Args&&... args) {
        Entity& entity = AddEntity();
        entity.AddComponent<ComponentType>(std::forward<Args>(args)...);

        return entity;
    }

    template <typename... ComponentTypes>
    Entity& World::AddEntityWithComponents(bool enabled) {
        Entity& entity = AddEntity();
        entity.AddComponents<ComponentTypes...>();

        return entity;
    }

    template <typename... ComponentTypes>
    std::vector<Entity*> World::GetEntitiesWithComponents() const {
        static_assert((std::is_base_of_v<Component, ComponentTypes> && ...),
            "[Error] The components must all be derived from Fl::Component.");

        std::vector<Entity*> entities;

        for (const EntityPtr& entity : m_Entities) {
            if ((entity->HasComponent<ComponentTypes>() && ...)) {
                entities.emplace_back(entity.get());
            }
        }

        return entities;
    }
}
