// Copyright (C) 2025 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

namespace Fl {
    inline const Bitset& System::GetAcceptedComponents() const {
        return m_AcceptedComponents;
    }

    template <typename SystemType>
    U64 System::GetId() {
        static_assert(std::is_base_of_v<System, SystemType>,
                      "[Error] The fetched id must be of a class derived from Fl::System");
        static_assert(!std::is_same_v<System, SystemType>,
                      "[Error] You have to use System::GetId only on a derived class and not on Fl::System.");

        static const U64 id = s_MaxId++;
        return id;
    }

    inline bool System::Update(const FrameTimeInfo& timeInfo) {
        return true;
    }

    inline void System::Destroy() {
    }

    template <typename... ComponentTypes>
    void System::RegisterComponents() {
        m_AcceptedComponents.SetBit(Component::GetId<ComponentTypes>()...);
    }

    template <typename... ComponentTypes>
    void System::UnregisterComponents() {
        (m_AcceptedComponents.SetBit(Component::GetId<ComponentTypes>(), false), ...);
    }
}
