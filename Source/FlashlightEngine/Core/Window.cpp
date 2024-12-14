// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#include <FlashlightEngine/Core/Window.hpp>

#include <FlashlightEngine/Core/Logger.hpp>

namespace FlashlightEngine {
    Window::Window(const WindowProperties& properties)
        : m_Data(properties.Width, properties.Height, properties.Title), m_Window(nullptr) {
        if (!glfwInit()) {
            Log::Critical("[CORE] Failed to initialize GLFW.");
            throw std::runtime_error("Failed to initialize GLFW.");
        }

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        m_Window = glfwCreateWindow(
            static_cast<Int32>(properties.Width),
            static_cast<Int32>(properties.Height),
            properties.Title.c_str(),
            nullptr,
            nullptr);

        if (!m_Window) {
            Log::Critical("[CORE] Failed to create window.");
            glfwTerminate();
            throw std::runtime_error("Failed to create window.");
        }

        glfwSetWindowUserPointer(m_Window, &m_Data);

        Log::Info("[CORE] Window '{0}' created with size ({1}, {2})",
                  properties.Title,
                  properties.Width,
                  properties.Height
        );

        // ===================== CALLBACKS ===================== //
        glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, const Int32 width, const Int32 height) -> void {
            auto* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            data->Width = static_cast<UInt32>(width);
            data->Height = static_cast<UInt32>(height);

            if (data->OnResizeCallback) {
                data->OnResizeCallback(width, height);
            }
        });
    }

    Window::~Window() {
        if (m_Window) {
            glfwDestroyWindow(m_Window);
        }

        glfwTerminate();
    }

    void Window::Update() {
        glfwPollEvents();
    }
}
