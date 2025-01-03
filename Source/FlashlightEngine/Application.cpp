// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#include <FlashlightEngine/Application.hpp>

#include <FlashlightEngine/Core/Time.hpp>

#include <FreeImage.h>

namespace FlashlightEngine {
    Application* Application::m_Instance = nullptr;

    Application::Application(const UInt32 width, const UInt32 height, const std::string& title) {
        FlAssert(!m_Instance, "Application already exists!");
        m_Instance = this;

        FreeImage_Initialise();

        WindowProperties properties{};
        properties.Width = width;
        properties.Height = height;
        properties.Title = title;

        m_Window = std::make_shared<Window>(properties);

        m_Window->SetEventCallback([this](Event& e) {
            EventDispatcher dispatcher(e);
            dispatcher.Dispatch<WindowCloseEvent>(FL_BIND_EVENT(Application::OnWindowClose));
            dispatcher.Dispatch<WindowResizeEvent>(FL_BIND_EVENT(Application::OnWindowResize));
            OnEvent(e);
        });

        m_Renderer = std::make_shared<Renderer>(m_Window);

        m_Running = true;
    }

    Application::~Application() {
        m_Window.reset();
        glfwTerminate();
    }

    void Application::Run() {
        while (m_Running) {
            Time::m_DeltaTime = glfwGetTime() - Time::m_LastFrameTime;
            Time::m_LastFrameTime += Time::m_DeltaTime.AsSeconds();

            m_Window->OnUpdate();

            OnUpdate();

            OnRender();
        }
    }

    bool Application::OnWindowClose([[maybe_unused]] WindowCloseEvent& e) {
        m_Running = false;
        return true;
    }

    bool Application::OnWindowResize(const WindowResizeEvent& e) const {
        m_Renderer->OnResize(e.GetWidth(), e.GetHeight());
        return true;
    }
}
