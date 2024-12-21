// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#include <FlashlightEngine/Application.hpp>

namespace FlashlightEngine {
    Application* Application::m_Instance = nullptr;

    Application::Application(const UInt32 width, const UInt32 height, const std::string& title) {
        FlAssert(!m_Instance, "Application already exists!");
        m_Instance = this;
        WindowProperties properties{};
        properties.Width = width;
        properties.Height = height;
        properties.Title = title;

        m_Window = std::make_shared<Window>(properties);

        m_Window->SetEventCallback([this](Event& e) {
            EventDispatcher dispatcher(e);
            dispatcher.Dispatch<WindowCloseEvent>(FL_BIND_EVENT(Application::OnWindowClose));
            OnEvent(e);
        });

        m_Running = true;
    }

    Application::~Application() {
    }

    void Application::Run() {
        while (m_Running) {
            m_Window->OnUpdate();

            OnUpdate();

            OnRender();
        }
    }

    bool Application::OnWindowClose([[maybe_unused]] WindowCloseEvent& e) {
        m_Running = false;
        return true;
    }
}
