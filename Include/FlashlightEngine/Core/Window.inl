// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

namespace FlashlightEngine {
    inline HWND Window::GetHandle() const {
        return m_HWnd;
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
        return m_Data.IsOpen;
    }

    inline void Window::Close() {
        m_Data.IsOpen = false;
    }

    inline void Window::OnResize(const std::function<void(UInt32, UInt32)>& callback) {
        m_Data.OnResizeCallback = callback;
    }
}
