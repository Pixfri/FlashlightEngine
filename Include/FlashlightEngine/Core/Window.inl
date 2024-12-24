// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

namespace FlashlightEngine {
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

    inline VideoMode Window::GetVideoMode() const {
        return {static_cast<Int32>(m_Data.Width), static_cast<Int32>(m_Data.Height)};
    }

    inline void Window::SetEventCallback(const std::function<void(Event&)>& callback) {
        m_Data.EventCallback = callback;
    }

    inline bool Window::IsFocused() const {
        return m_Data.HasFocus;
    }

    inline bool Window::IsVSync() const {
        return m_Data.VSync;
    }

    inline void Window::SwapchainInvalidated() {
        m_Data.ShouldInvalidateSwapchain = false;
    }

    inline bool Window::ShouldInvalidateSwapchain() const {
        return m_Data.ShouldInvalidateSwapchain;
    }

    inline bool Window::IsFullscreen() const {
        return m_Data.Fullscreen;
    }

    inline GLFWwindow* Window::GetNativeWindow() const {
        return m_Window;
    }
}
