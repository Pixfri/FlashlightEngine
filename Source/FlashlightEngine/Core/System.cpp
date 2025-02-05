// Copyright (C) 2025 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#include <FlashlightEngine/Core/System.hpp>

namespace Fl {
    bool System::ContainsEntity(const Entity& entity) const noexcept {
        for (const auto entityPtr : m_Entities) {
            if (entityPtr->GetId() == entity.GetId()) {
                return true;
            }
        }

        return false;
    }

    void System::LinkEntity(const EntityPtr& entity) {
        m_Entities.emplace_back(entity.get());
    }

    void System::UnlinkEntity(const EntityPtr& entity) {
        for (U64 entityIndex = 0; entityIndex < m_Entities.size(); entityIndex++) {
            if (m_Entities[entityIndex]->GetId() == entity->GetId()) {
                m_Entities.erase(m_Entities.begin() + static_cast<std::ptrdiff_t>(entityIndex));
                break;
            }
        }
    }
}
