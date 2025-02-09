// Copyright (C) 2025 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

namespace Fl {
	inline const std::vector<ApplicationComponentPtr>& Application::GetAppComponents() const {
		return m_Components;
	}

	inline std::vector<ApplicationComponentPtr>& Application::GetAppComponents() {
		return m_Components;
	}

	inline const std::vector<WorldPtr>& Application::GetWorlds() const {
		return m_Worlds;
	}

	inline std::vector<WorldPtr>& Application::GetWorlds() {
		return m_Worlds;
	}

	inline FrameTimeInfo Application::GetTimeInfo() const {
		return m_TimeInfo;
	}

	inline void Application::SetFixedTimeStep(const F32 fixedTimeStep) {
		FlAssert(fixedTimeStep > 0.f, "Fixed time step {} must be positive.");

		m_TimeInfo.SubStepTime = fixedTimeStep;
	}

	template <typename... Args>
	World& Application::AddWorld(Args&&... args) {
		m_Worlds.emplace_back(std::make_unique<World>(std::forward<Args>(args)...));
		m_ActiveWorlds.SetBit(m_Worlds.size() - 1);

		return *m_Worlds.back();
	}

	template <typename ComponentType, typename... Args>
	ComponentType& Application::AddComponent(Args&&... args) {
		static_assert(std::is_base_of_v<ApplicationComponent, ComponentType>, "[Error] The application component must be derived from Fl::ApplicationComponent.");

		const U64 componentId = ApplicationComponent::GetId<ComponentType>();

		if (componentId >= m_Components.size()) {
			m_Components.resize(componentId + 1);
		}

		m_Components[componentId] = std::make_unique<ComponentType>(std::forward<Args>(args)...);
		m_ActiveComponents.SetBit(componentId);

		return static_cast<ComponentType&>(*m_Components[componentId]);
	}

	template <typename ComponentType>
	bool Application::HasComponent() const {
		static_assert(std::is_base_of_v<ApplicationComponent, ComponentType>, "[Error] The application component must be derived from Fl::ApplicationComponent.");

		const U64 componentId = ApplicationComponent::GetId<ComponentType>();

		return ((componentId < m_Components.size()) && m_Components[componentId]);
	}

	template <typename ComponentType>
	const ComponentType& Application::GetComponent() const {
		static_assert(std::is_base_of_v<ApplicationComponent, ComponentType>, "[Error] The application component must be derived from Fl::ApplicationComponent.");

		if (HasComponent<ComponentType>()) {
			return static_cast<const ComponentType&>(*m_Components[ApplicationComponent::GetId<ComponentType>()]);
		}

		throw std::runtime_error("[Error] No component available for the given type.");
	}

	template <typename ComponentType>
	ComponentType& Application::GetComponent() {
		return const_cast<ComponentType&>(static_cast<const Application*>(this)->GetComponent<ComponentType>());
	}

	template <typename ComponentType>
	void Application::RemoveComponent() {
		static_assert(std::is_base_of_v<ApplicationComponent, ComponentType>, "[Error] The application component must be derived from Fl::ApplicationComponent.");

		if (HasComponent<ComponentType>()) {
			m_Components[ApplicationComponent::GetId<ComponentType>()].reset();
		}
	}

	template <typename FuncType>
	void Application::Run(FuncType&& callback) {
		while (RunOnce()) {
			callback();
		}
	}

	inline void Application::Quit() {
		m_IsRunning = false;
	}
}
