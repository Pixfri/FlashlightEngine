// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#ifndef FL_CORE_LOGGER_HPP
#define FL_CORE_LOGGER_HPP

#include <FlashlightEngine/Types.hpp>

#include <spdlog/spdlog.h>

namespace FlashlightEngine {
    class Logger {
    public:
        Logger() = delete;
        ~Logger() = delete;

        Logger(const Logger&) = delete;
        Logger(Logger&&) = delete;

        static void Init();
        inline static spdlog::logger* GetLoggerInstance();

        Logger& operator=(const Logger&) = delete;
        Logger& operator=(Logger&&) = delete;
    
    private:
        static std::shared_ptr<spdlog::logger> m_Logger;
    };

    namespace Log {
        template <typename... Args>
        void Trace(Args&&... args);

        template <typename... Args>
        void Debug(Args&&... args);

        template <typename... Args>
        void Info(Args&&... args);

        template <typename... Args>
        void Warn(Args&&... args);

        template <typename... Args>
        void Error(Args&&... args);

        template <typename... Args>
        void Critical(Args&&... args);
    }
}

#include <FlashlightEngine/Core/Logger.inl>

#endif // FL_CORE_LOGGER_HPP
