// Copyright (C) 2025 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

namespace Fl {
    inline Entity::Entity(const U64 index, const bool enabled)
        : m_Id{index}, m_Enabled{enabled} {
    }

    inline U64 Entity::GetId() const noexcept {
        return m_Id;
    }

    inline bool Entity::IsEnabled() const noexcept {
        return m_Enabled;
    }

    inline const std::vector<ComponentPtr>& Entity::GetComponents() const noexcept {
        return m_Components;
    }

    inline const Bitset& Entity::GetEnabledComponents() const noexcept {
        return m_EnabledComponents;
    }

    template <typename... Args>
    EntityPtr Entity::Create(Args&&... args) {
        return std::make_unique<Entity>(std::forward<Args>(args)...);
    }

    inline void Entity::Enable(const bool status) noexcept {
        m_Enabled = status;
    }

    inline void Entity::Disable() noexcept {
        Enable(false);
    }

    template <typename ComponentType, typename... Args>
    ComponentType& Entity::AddComponent(Args&&... args) {
        static_assert(std::is_base_of_v<Component, ComponentType>,
                      "[Error] The component must be derived from Fl::Component");

        const U64 componentId = Component::GetId<ComponentType>();

        if (componentId >= m_Components.size()) {
            m_Components.resize(componentId + 1);
        }

        m_Components[componentId] = std::make_unique<ComponentType>(std::forward<Args>(args)...);
        m_EnabledComponents.SetBit(componentId);

        return static_cast<ComponentType&>(*m_Components[componentId]);
    }

    template <typename ComponentType>
    std::tuple<ComponentType&> Entity::AddComponents() {
        static_assert(std::is_base_of_v<Component, ComponentType>,
                      "[Error] The component must be derived from Fl::Component");

        return std::forward_as_tuple(AddComponent<ComponentType>());
    }

    template <typename ComponentType1, typename ComponentType2, typename... ComponentTypes>
    std::tuple<ComponentType1&, ComponentType2&, ComponentTypes...> Entity::AddComponents() {
        static_assert(std::is_base_of_v<Component, ComponentType1>,
                      "[Error] The component must be derived from Fl::Component");

        return std::tuple_cat(std::forward_as_tuple(AddComponent<ComponentType1>()),
                              AddComponents<ComponentType2, ComponentTypes...>());
    }

    template <typename ComponentType>
    bool Entity::HasComponent() const {
        static_assert(std::is_base_of_v<Component, ComponentType>,
                              "[Error] The component must be derived from Fl::Component");

        const U64 componentId = Component::GetId<ComponentType>();

        return ((componentId < m_Components.size()) && m_EnabledComponents[componentId]);
    }

    template <typename ComponentType>
    const ComponentType& Entity::GetComponent() const {
        static_assert(std::is_base_of_v<Component, ComponentType>,
                      "[Error] The component must be derived from Fl::Component");

        if (HasComponent<ComponentType>()) {
            return static_cast<const ComponentType&>(*m_Components[Component::GetId<ComponentType>()]);
        }

        throw std::runtime_error("[Error] No component available with the given type.");
    }

    template <typename ComponentType>
    ComponentType& Entity::GetComponent() {
        static_assert(std::is_base_of_v<Component, ComponentType>,
                      "[Error] The component must be derived from Fl::Component");

        return const_cast<ComponentType&>(static_cast<const Entity*>(this)->GetComponent<ComponentType>());
    }

    template <typename ComponentType>
    void Entity::RemoveComponent() {
        static_assert(std::is_base_of_v<Component, ComponentType>,
                      "[Error] The component must be derived from Fl::Component");

        if (HasComponent<ComponentType>()) {
            const U64 componentId = Component::GetId<ComponentType>();

            m_Components[componentId].reset();
            m_EnabledComponents.SetBit(componentId, false);
        }
    }

}
