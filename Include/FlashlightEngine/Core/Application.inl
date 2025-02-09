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

	template <typename... Args>
	ApplicationComponent& Application::AddComponent(Args&&... args) {
		m_Components.emplace_back(std::make_unique<ApplicationComponent>(std::forward<Args>(args)...));
		m_ActiveComponents.SetBit(m_Components.size() - 1);

		return *m_Components.back();
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
