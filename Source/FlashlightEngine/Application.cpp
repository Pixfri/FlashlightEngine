// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#include <FlashlightEngine/Application.hpp>

#include <FlashlightEngine/Core/Logger.hpp>

namespace FlashlightEngine {
    Application::Application(const UInt32 width, const UInt32 height) {
        Logger::Init();

        m_Window = std::make_shared<Window>(WindowProperties{ width, height, "Flashlight Engine" });

        m_Window->OnResize([](const UInt32 newWidth, const UInt32 newHeight) -> void {
            FlUnused(newWidth);
            FlUnused(newHeight);
            // Log::Debug("Window resized: new size is ({0}, {1})", newWidth, newHeight);
        });

        Log::Info("Application initialized");
    }

    void Application::Run() const {
        while (m_Window->IsOpen()) {
            m_Window->Update();
        }

        Log::Info("Application closed");
    }
}
