// Copyright (C) 2025 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#include <FlashlightEngine/Core/Application.hpp>

#include <FlashlightEngine/Core/Assert.hpp>
#include <FlashlightEngine/Core/Profiler.hpp>

#include <spdlog/async.h>
#include <spdlog/cfg/env.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Fl {
    bool Application::m_IsLoggerSetup = false;

    Application::Application(const U64 worldCount) {
        m_Worlds.reserve(worldCount);

        if (!m_IsLoggerSetup) {
            SetupLogger();
        }
    }

    void Application::Run() {
        spdlog::debug("[Application] Running...");
        
        while (RunOnce());

        spdlog::debug("[Application] Exiting...");
    }

    bool Application::RunOnce() {
        FL_PROFILE("Application::RunOnce");

        const auto currentTime = std::chrono::system_clock::now();
        m_TimeInfo.DeltaTime = std::chrono::duration<F32>(currentTime - m_LastFrameTime).count();
        m_TimeInfo.GlobalTime += m_TimeInfo.DeltaTime;
        m_LastFrameTime = currentTime;

        m_TimeInfo.SubStepCount = 0;
        m_RemainingTime += m_TimeInfo.DeltaTime;

        while (m_RemainingTime >= m_TimeInfo.SubStepTime) {
            ++m_TimeInfo.SubStepCount;
            m_RemainingTime -= m_TimeInfo.SubStepTime;
        }

        for (U64 componentIndex = 0; componentIndex < m_Components.size(); ++componentIndex) {
            if (!m_ActiveComponents[componentIndex]) {
                continue;
            }

            if (!m_Components[componentIndex] || !m_Components[componentIndex]->Update(m_TimeInfo)) {
                m_ActiveComponents.SetBit(componentIndex, false);
            }
        }

        for (U64 worldIndex = 0; worldIndex < m_Worlds.size(); ++worldIndex) {
            if (!m_ActiveWorlds[worldIndex]) {
                continue;
            }

            if (!m_Worlds[worldIndex]->Update(m_TimeInfo)) {
                m_ActiveWorlds.SetBit(worldIndex, false);
            }
        }


        FL_PROFILER_FRAMEMARK;

        return (m_IsRunning && (!m_ActiveWorlds.IsEmpty() || !m_ActiveComponents.IsEmpty()));
    }

    void Application::SetupLogger() {
        spdlog::init_thread_pool(8192, 1);
        const auto stdoutSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        const auto fileSink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
            "FlashlightEngine.log", 1024 * 1024 * 10, 3
        );
        std::vector<spdlog::sink_ptr> sinks{stdoutSink, fileSink};
        m_AppLogger = std::make_shared<spdlog::async_logger>("FlashlightEngine",
                                                             sinks.begin(), sinks.end(),
                                                             spdlog::thread_pool(),
                                                             spdlog::async_overflow_policy::block);

        spdlog::register_logger(m_AppLogger);
        spdlog::set_default_logger(m_AppLogger);
        spdlog::set_pattern("[%H:%M:%S %z] [%n] [%^---%L---%$] [thread %t] %v");

        spdlog::cfg::load_env_levels();

        m_IsLoggerSetup = true;
    }
}
