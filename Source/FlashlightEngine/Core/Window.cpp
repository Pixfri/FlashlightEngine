// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#include <FlashlightEngine/Core/Window.hpp>

#include <FlashlightEngine/Core/CoreUtils.hpp>

#include <spdlog/spdlog.h>

#include <stb/stb_image.h>

#include <GLFW/glfw3.h>

namespace FlashlightEngine {
    bool Window::m_IsGlfwInit = false;

    Window::Window(const WindowProperties& properties) {
        Init(properties);
    }

    Window::~Window() {
        Quit();
    }

    void Window::Init(const WindowProperties& properties) {
        m_Data.Title = properties.Title;
        m_Data.Width = properties.Width;
        m_Data.Height = properties.Height;

        if (!m_IsGlfwInit) {
            FlAssert(glfwInit(), "[GLFW] Failed to initialize GLFW.");
            m_IsGlfwInit = true;
        }

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        m_Window = glfwCreateWindow(static_cast<Int32>(m_Data.Width), static_cast<Int32>(m_Data.Height),
                                    m_Data.Title.c_str(), nullptr, nullptr);
        FlAssert(m_Window, "[GLFW] Failed to create GLFW window.");

#ifndef __unix__
        Int32 texWidth{}, texHeight{}, texChannels{};
        stbi_uc* pixels = stbi_load(properties.IconPath.string().c_str(), &texWidth, &texHeight, &texChannels,
                                    STBI_rgb_alpha);

        const GLFWimage icon = {texWidth, texHeight, pixels};
        glfwSetWindowIcon(m_Window, 1, &icon);

        stbi_image_free(pixels);
#endif

        glfwSetWindowUserPointer(m_Window, &m_Data);
        SetVSync(true);

        // --------------------------------------------- Glfw callbacks ---------------------------------------------

        glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
            const auto data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            WindowCloseEvent event;
            data->EventCallback(event);
        });

        glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, const Int32 width, const Int32 height) {
            const auto data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            data->Width = static_cast<UInt32>(width);
            data->Height = static_cast<UInt32>(height);

            WindowResizeEvent event(width, height);
            data->EventCallback(event);
        });

        glfwSetWindowPosCallback(m_Window, [](GLFWwindow* window, const Int32 xPos, const Int32 yPos) {
            const auto data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            WindowMovedEvent event(xPos, yPos);
            data->EventCallback(event);
        });

        glfwSetKeyCallback(m_Window, [](GLFWwindow* window,
                                        const Int32 key,
                                        [[maybe_unused]] const Int32 scancode,
                                        const Int32 action,
                                        [[maybe_unused]] const Int32 mods) {
            const auto data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            switch (action) {
            case GLFW_PRESS:
                {
                    KeyDownEvent event(key, 0);
                    data->EventCallback(event);
                    break;
                }
            case GLFW_RELEASE:
                {
                    KeyUpEvent event(key);
                    data->EventCallback(event);
                    break;
                }
            case GLFW_REPEAT:
                {
                    KeyDownEvent event(key, 1);
                    data->EventCallback(event);
                    break;
                }
            default:
                spdlog::warn("[GLFW] Unknown key action.");
                break;
            }
        });

        glfwSetCharCallback(m_Window, [](GLFWwindow* window, const UInt32 keycode) {
            const auto data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            KeyTypedEvent event(static_cast<Int32>(keycode));
            data->EventCallback(event);
        });

        glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window,
                                                const Int32 button,
                                                const Int32 action,
                                                [[maybe_unused]] const Int32 mods) {
            const auto data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            switch (action) {
            case GLFW_PRESS:
                {
                    MouseButtonDownEvent event(button);
                    data->EventCallback(event);
                    break;
                }
            case GLFW_RELEASE:
                {
                    MouseButtonUpEvent event(button);
                    data->EventCallback(event);
                    break;
                }
            default:
                spdlog::warn("[GLFW] Unknown mouse button action.");
                break;
            }
        });

        glfwSetScrollCallback(m_Window, [](GLFWwindow* window, const Float64 xOffset, const Float64 yOffset) {
            const auto data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            MouseScrolledEvent event(static_cast<Float32>(xOffset), static_cast<Float32>(yOffset));
            data->EventCallback(event);
        });

        glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, const Float64 xPos, const Float64 yPos) {
            const auto data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            MouseMotionEvent event(static_cast<Float32>(xPos), static_cast<Float32>(yPos));
            data->EventCallback(event);
        });

        glfwSetWindowFocusCallback(m_Window, [](GLFWwindow* window, const Int32 focused) {
            const auto data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            WindowFocusEvent event(focused);
            data->EventCallback(event);
        });

        spdlog::info("[GLFW] Window created with dimensions: {}x{}.", properties.Width, properties.Height);
    }

    void Window::Quit() const {
        spdlog::info("[GLFW] Window destroyed.");
        glfwDestroyWindow(m_Window);
    }

    void Window::OnUpdate() {
        glfwPollEvents();
    }

    static void WinInitPos(GLFWwindow* win, Int32& x, Int32& y) {
        glfwGetWindowPos(win, &x, &y);
    }

    void Window::UpdateFullscreenMode() {
        if (m_Data.ShouldUpdateFullscreenMode) {
            static Int32 xPos, yPos;
            WinInitPos(m_Window, xPos, yPos);

            static UInt32 width, height;
            GetSize(width, height);

            if (m_Data.Fullscreen) {
                const auto monitor = glfwGetPrimaryMonitor();
                const auto mode = glfwGetVideoMode(monitor);

                static Int32 iWidth, iHeight;

                // Store windows pos data to restore later
                glfwGetWindowPos(m_Window, &xPos, &yPos);
                glfwGetWindowSize(m_Window, &iWidth, &iHeight);

                glfwSetWindowMonitor(m_Window, monitor, 0, 0, mode->width, mode->height,
                                     IsVSync() ? mode->refreshRate : 0);
            } else {
                glfwSetWindowMonitor(m_Window, nullptr, xPos, yPos, static_cast<Int32>(width),
                                     static_cast<Int32>(height), 0);
            }

            m_Data.ShouldUpdateFullscreenMode = false;
        }
    }

    void Window::SetFullscreen(const bool fullscreen) {
        m_Data.ShouldUpdateFullscreenMode = true;
        m_Data.Fullscreen = fullscreen;
    }

    void Window::SetVSync(const bool enabled) {
        glfwSwapInterval(enabled);
        m_Data.VSync = enabled;
    }
}
