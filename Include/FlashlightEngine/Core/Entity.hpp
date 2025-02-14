// Copyright (C) 2025 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#ifndef FL_CORE_ENTITY_HPP
#define FL_CORE_ENTITY_HPP

#include <FlashlightEngine/Prerequisites.hpp>

#include <FlashlightEngine/Core/Component.hpp>
#include <FlashlightEngine/Data/Bitset.hpp>

#include <memory>
#include <stdexcept>

namespace Fl {
    class Entity;
    using EntityPtr = std::unique_ptr<Entity>;

    /**
     * @brief Represents an entity in a world with components attached to it.
     */
    class Entity {
    public:
        inline explicit Entity(U64 index, bool enabled = true);
        ~Entity() = default;

        Entity(const Entity&) = delete;
        Entity(Entity&&) noexcept = delete;

        /**
         * @brief Gets the entity id in the world.
         * @return The entity's id.
         */
        [[nodiscard]] inline U64 GetId() const noexcept;

        /**
         * @brief Checks if the entity is active (enabled) or not.
         * @return Whether the entity is active or not.
         */
        [[nodiscard]] inline bool IsEnabled() const noexcept;

        /**
         * @brief Gets the components attached to the entity.
         * @return A reference to the vector containing the pointers to the components attached to the entity.
         */
        [[nodiscard]] inline const std::vector<ComponentPtr>& GetComponents() const noexcept;

        /**
         * @brief Gets the enable components.
         * @return A @ref Fl::Bitset containing a bit for each component to tell if it's active or not.
         */
        [[nodiscard]] inline const Bitset& GetEnabledComponents() const noexcept;

        /**
         * @brief Creates an entity with the given parameters.
         * @tparam Args The different arguments types.
         * @param args The different arguments to be forwarded.
         * @return A pointer to the new entity.
         */
        template <typename... Args>
        static EntityPtr Create(Args&&... args);

        /**
         * @brief Enables/disables the entity.
         * @param status If the entity should be active or not.
         */
        inline void Enable(bool status = true) noexcept;

        /**
         * @brief Disable the entity. This is basically calling @ref Enable(false).
         */
        inline void Disable() noexcept;

        /**
         * @brief Adds a new component to the entity.
         * @tparam ComponentType Type of the component to be added.
         * @tparam Args Type of the arguments to pass to the component at creation.
         * @param args Arguments to pass to the component at creation.
         * @return A reference to the added component.
         */
        template <typename ComponentType, typename... Args>
        ComponentType& AddComponent(Args&&... args);

        /**
         * @brief Adds a last component to the entity.
         * @tparam ComponentType Type of the last component to be added.
         * @return A tuple containing a reference to the last newly added component.
         */
        template <typename ComponentType>
        std::tuple<ComponentType&> AddComponents();

        /**
         * @brief Adds multiple components at once to the entity.
         * @tparam ComponentType1 Type of the first component to add.
         * @tparam ComponentType2 Type of the second component to add.
         * @tparam ComponentTypes Type of the other components to add.
         * @return A tuple containing references to the added components.
         */
        template <typename ComponentType1, typename ComponentType2, typename... ComponentTypes>
        std::tuple<ComponentType1&, ComponentType2&, ComponentTypes...> AddComponents();

        /**
         * @brief Checks if the entity has a given component.
         * @tparam ComponentType Type of the component to be checked.
         * @return Whether the entity has the given component or not.
         */
        template <typename ComponentType>
        bool HasComponent() const;

        /**
         * @brief Gets a given component from the entity.
         * @tparam ComponentType Type of the component to get.
         * @return A reference to the component.
         * @throws std::runtime_error if the entity doesn't have the component.
         */
        template <typename ComponentType>
        const ComponentType& GetComponent() const;

        /**
         * @brief Gets a given component from the entity.
         * @tparam ComponentType Type of the component to get.
         * @return A reference to the component.
         * @throws std::runtime_error if the entity doesn't have the component.
         */
        template <typename ComponentType>
        ComponentType& GetComponent();

        /**
         * @brief Removes the given component from the entity.
         * @tparam ComponentType Type of the component to remove.
         */
        template <typename ComponentType>
        void RemoveComponent();

        Entity& operator=(const Entity&) = delete;
        Entity& operator=(Entity&&) noexcept = delete;

    private:
        U64 m_Id{};
        bool m_Enabled{};
        std::vector<ComponentPtr> m_Components{};
        Bitset m_EnabledComponents{};
    };
}

#include <FlashlightEngine/Core/Entity.inl>

#endif // FL_CORE_ENTITY_HPP
