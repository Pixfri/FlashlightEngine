// Copyright (C) 2025 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#ifndef FL_CORE_APPLICATION_HPP
#define FL_CORE_APPLICATION_HPP

#include <FlashlightEngine/Prerequisites.hpp>

#include <spdlog/async_logger.h>

#include <memory>

namespace Fl {
    class Application final {
    public:
        Application();
        ~Application();

        static inline Application& GetInstance();

        Application(const Application&) = delete;
        Application(Application&&) = delete;

        Application& operator=(const Application&) = delete;
        Application& operator=(Application&&) = delete;
    
    private:
        static Application* s_AppInstance;

        std::shared_ptr<spdlog::async_logger> m_AppLogger;

        void SetupLogger();
    };
}

#include <FlashlightEngine/Core/Application.inl>

#endif // FL_CORE_APPLICATION_HPP
