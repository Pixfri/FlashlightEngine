// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

namespace FlashlightEngine {
    inline spdlog::logger* Logger::GetLoggerInstance() {
        return m_Logger.get();
    }

    namespace Log {
        template <typename... Args>
        void Trace(Args&&... args) {
            Logger::GetLoggerInstance()->trace(std::forward<Args>(args)...);
        }

        template <typename... Args>
        void Debug(Args&&... args) {
            Logger::GetLoggerInstance()->debug(std::forward<Args>(args)...);
        }

        template <typename... Args>
        void Info(Args&&... args) {
            Logger::GetLoggerInstance()->info(std::forward<Args>(args)...);
        }

        template <typename... Args>
        void Warn(Args&&... args) {
            Logger::GetLoggerInstance()->warn(std::forward<Args>(args)...);
        }

        template <typename... Args>
        void Error(Args&&... args) {
            Logger::GetLoggerInstance()->error(std::forward<Args>(args)...);
        }

        template <typename... Args>
        void Critical(Args&&... args) {
            Logger::GetLoggerInstance()->critical(std::forward<Args>(args)...);
        }
    }
}
