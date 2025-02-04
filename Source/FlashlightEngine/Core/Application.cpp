// Copyright (C) 2025 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#include <FlashlightEngine/Core/Application.hpp>

#include <FlashlightEngine/Core/Assert.hpp>

#include <spdlog/async.h>
#include <spdlog/cfg/env.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Fl {
    Application* Application::s_AppInstance = nullptr;

    Application::Application() {
        FlAssert(!s_AppInstance, "Error: Application already created.");

        SetupLogger();

        spdlog::info("Application created.");

        s_AppInstance = this;
    }

    Application::~Application() {
        s_AppInstance = nullptr;

        spdlog::info("Application destroyed.");
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
    }

}
