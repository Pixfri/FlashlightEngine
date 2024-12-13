// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#include <FlashlightEngine/Core/Logger.hpp>

#include <spdlog/sinks/stdout_color_sinks.h>

namespace FlashlightEngine {
    std::shared_ptr<spdlog::logger> Logger::m_Logger;

    void Logger::Init() {
        spdlog::set_pattern("[%H:%M:%S %z] [%n] [%^---%L---%$] [thread %t] %v");

        m_Logger = spdlog::stdout_color_mt("FlashlightEngine");
        m_Logger->set_level(spdlog::level::trace);
    }
}
