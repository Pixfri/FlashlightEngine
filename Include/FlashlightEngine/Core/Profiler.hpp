// Copyright (C) 2025 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#ifndef FL_CORE_TRACY_HPP
#define FL_CORE_TRACY_HPP

#include <FlashlightEngine/Prerequisites.hpp>

#include <unordered_map>

#ifdef TRACY_ENABLE

#include <tracy/Tracy.hpp>

// malloc and free are used by Tracy to provide memory profiling
void* operator new(size_t count);
void operator delete(void* ptr) noexcept;

#define FL_PROFILE(name) ZoneScopedN(name)
#define FL_PROFILER_FRAMEMARK FrameMark

#else

#define FL_PROFILE_SCOPE(name)
#define FL_PROFILER_FRAMEMARK

#endif

namespace Fl {
	enum class PlotType : U8 {
		Number,
		Percentage,
		Memory
	};

#ifdef TRACY_ENABLE
	// tracy::PlotFormatType is not define if TRACY_ENABLE is not defined
	// so we need to define a function to convert our enum to the tracy enum.
	namespace {
		inline tracy::PlotFormatType ToTracyPlotFormat(PlotType type) {
			switch (type) {
			case PlotType::Number:
				return tracy::PlotFormatType::Number;
			case PlotType::Percentage:
				return tracy::PlotFormatType::Percentage;
			case PlotType::Memory:
				return tracy::PlotFormatType::Memory;
			}
		}
	}

#define FL_TO_TRACY_PLOT_FORMAT(name) ToTracyPlotFormat(name)
#else
#define FL_TO_TRACY_PLOT_FORMAT(name)
#endif

	// Create plots
	template <typename T, PlotType PT = PlotType::Number>
	class Plot {
	public:
		static void AddPlot(const char* name, T value) {
			auto* p = GetInstance();
			p->m_Plots[name] = value;
			UpdateTracyPlot(name, value);
		}

		static void Increment(const char* name, T amount) {
			auto* p = GetInstance();
			p->m_Plots[name] += amount;
			UpdateTracyPlot(name, p->m_Plots[name]);
		}

		static void Decrement(const char* name, T amount) {
			auto* p = GetInstance();
			p->m_Plots[name] -= amount;
			UpdateTracyPlot(name, p->m_Plots[name]);
		}

		static void Reset(const char* name) {
			auto* p = GetInstance();
			p->m_Plots[name] = T{};
			UpdateTracyPlot(name, p->m_Plots[name]);
		}

	private:
		static void UpdateTracyPlot(const char* name, T value) {
#ifdef TRACY_ENABLE
			TracyPlot(name, value);
			TracyPlotConfig(name, FL_TO_TRACY_PLOT_FORMAT(PT), true, true, 0);
#endif
		}

		static Plot* GetInstance() {
			static_assert((std::is_same_v<T, I64> || std::is_same_v<T, F64> || std::is_same_v<T, F32>), "PlotStore only supports I64, F64 and F32.");
			static Plot instance;
			return &instance;
		}

		std::unordered_map<const char*, T> m_Plots;
	};
}

#endif // FL_CORE_TRACY_HPP
