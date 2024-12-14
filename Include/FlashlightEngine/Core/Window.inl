// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

namespace FlashlightEngine {
    inline GLFWwindow* Window::GetHandle() const {
        return m_Window;
    }

    inline UInt32 Window::GetWidth() const {
        return m_Data.Width;
    }

    inline UInt32 Window::GetHeight() const {
        return m_Data.Height;
    }

    inline void Window::GetSize(UInt32& width, UInt32& height) const {
        width = m_Data.Width;
        height = m_Data.Height;
    }

    inline std::string Window::GetTitle() const {
        return m_Data.Title;
    }

    inline bool Window::IsOpen() const {
        return !glfwWindowShouldClose(m_Window);
    }

    inline void Window::Close() const {
        glfwSetWindowShouldClose(m_Window, true);
    }

    inline void Window::OnResize(const std::function<void(UInt32, UInt32)>& callback) {
        m_Data.OnResizeCallback = callback;
    }
}
