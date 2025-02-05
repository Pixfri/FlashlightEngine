// Copyright (C) 2025 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#include <FlashlightEngine/Core/World.hpp>

#include <FlashlightEngine/Core/Profiler.hpp>

namespace Fl {
    Entity& World::AddEntity(bool enabled) {
        m_Entities.emplace_back(Entity::Create(m_MaxEntityIndex++, enabled));
        m_ActiveEntityCount += enabled;

        return *m_Entities.back();
    }

    void World::RemoveEntity(const Entity& entity) {
        const auto it = std::ranges::find_if(
            m_Entities,
            [&entity](const EntityPtr& entityPtr) {
                return (&entity == entityPtr.get());
            }
        );

        if (it == m_Entities.end()) {
            throw std::invalid_argument("[Error] The entity isn't part of this world.");
        }

        for (const SystemPtr& system : m_Systems) {
            system->UnlinkEntity(*it);
        }

        m_Entities.erase(it);
    }

    bool World::Update(const FrameTimeInfo& timeInfo) {
        FL_PROFILE("World::Update");

        Refresh();

        for (U64 systemIndex = 0; systemIndex < m_Systems.size(); ++systemIndex) {
            if (!m_ActiveSystems[systemIndex]) {
                continue;
            }

            const bool isSystemActive = m_Systems[systemIndex]->Update(timeInfo);

            if (!isSystemActive) {
                m_ActiveSystems.SetBit(systemIndex, false);
            }
        }

        return !m_ActiveSystems.IsEmpty();
    }

    void World::Refresh() {
        FL_PROFILE("World::Refresh");

        if (m_Entities.empty()) {
            return;
        }

        SortEntities();

        for (U64 entityIndex = 0; entityIndex < m_Entities.size(); ++entityIndex) {
            const EntityPtr& entity = m_Entities[entityIndex];

            if (!entity->IsEnabled()) {
                continue;
            }

            for (U64 systemIndex = 0; systemIndex < m_Systems.size(); ++systemIndex) {
                const SystemPtr& system = m_Systems[systemIndex];

                if (system == nullptr || !m_ActiveSystems[systemIndex]) {
                    continue;
                }

                const Bitset matchingComponents = system->GetAcceptedComponents() & entity->GetEnabledComponents();

                // If the system does not contain the entity, check if it should (if it possesses the accepted components); if yes, link it
                // else, if the system contains the entity but should not, unlink it.
                if (!system->ContainsEntity(*entity)) {
                    if (!matchingComponents.IsEmpty()) {
                        system->LinkEntity(entity);
                    }
                } else {
                    if (matchingComponents.IsEmpty()) {
                        system->UnlinkEntity(entity);
                    }
                }
            }
        }
    }

    void World::Destroy() {
        FL_PROFILE("World::Destroy");

        // Entities must be released before the systems, since their destruction may depend on those.
        m_Entities.clear();
        m_ActiveEntityCount = 0;
        m_MaxEntityIndex = 0;

        // This means that no entity must be used in any system destructor, since they will all be invalid
        // Their list is thus cleared to avoid any invalid usage
        for (const SystemPtr& system : m_Systems) {
            if (system) {
                system->m_Entities.clear();
            }
        }

        m_Systems.clear();
        m_ActiveSystems.Clear();
    }

    void World::SortEntities() {
        FL_PROFILE("World::SortEntities");

        // Reorganizing the entities, swapping enabled & disabled ones so that the enabled ones are in front
        auto firstEntity = m_Entities.begin();
        auto lastEntity = m_Entities.end() - 1;

        while (firstEntity != lastEntity) {
            // Iterating from the beginning to the end, trying to find a disabled entity
            if ((*firstEntity)->IsEnabled()) {
                ++firstEntity;
                continue;
            }

            // Iterating from the end to the beginning, trying to find an enabled entity
            while (firstEntity != lastEntity && (*lastEntity == nullptr || !(*lastEntity)->IsEnabled())) {
                --lastEntity;
            }

            // If both iterators are equal to each other, the list is sorted
            if (firstEntity == lastEntity) {
                break;
            }

            std::swap(*firstEntity, *lastEntity);
            --lastEntity;
        }

        m_ActiveEntityCount = static_cast<U64>(std::distance(m_Entities.begin(), lastEntity) + 1);
    }
}
