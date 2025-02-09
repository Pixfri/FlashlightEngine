// Copyright (C) 2025 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

namespace Fl {
	template <typename AppComponentType>
	U64 ApplicationComponent::GetId() {
		static_assert(std::is_base_of_v<ApplicationComponent, AppComponentType>,
			"[Error] The fetched id must be of a class derived from Fl::ApplicationComponent");
		static_assert(!std::is_same_v<ApplicationComponent, AppComponentType>,
			"[Error] You have to use ApplicationComponent::GetId only on a derived class and not on Fl::ApplicationComponent.");

		static const U64 id = s_MaxId++;
		return id;
	}

	inline bool ApplicationComponent::Update([[maybe_unused]] const FrameTimeInfo& timeInfo) {
		return true;
	}
}
