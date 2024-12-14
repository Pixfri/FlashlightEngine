// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#ifndef FL_CORE_WINDOW_HPP
#define FL_CORE_WINDOW_HPP

#include <FlashlightEngine/Types.hpp>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <Windows.h>

#include <functional>
#include <string>

namespace FlashlightEngine {
    struct WindowProperties {
        UInt32 Width, Height;
        std::string Title;
    };

    class Window {
    public:
        explicit Window(const WindowProperties& properties);
        ~Window();

        Window(const Window&) = delete;
        Window(Window&&) = delete;

        [[nodiscard]] inline HWND GetHandle() const;
        [[nodiscard]] inline UInt32 GetWidth() const;
        [[nodiscard]] inline UInt32 GetHeight() const;
        inline void GetSize(UInt32& width, UInt32& height) const;
        [[nodiscard]] inline std::string GetTitle() const;
        [[nodiscard]] inline bool IsOpen() const;

        void Update() const;
        void Close();
        inline void OnResize(const std::function<void(UInt32, UInt32)>& callback);

        Window& operator=(const Window&) = delete;
        Window& operator=(Window&&) = delete;
    
    private:
        struct WindowData {
            UInt32 Width, Height;
            std::string Title;
            std::function<void(UInt32, UInt32)> OnResizeCallback;
            bool IsOpen;
        } m_Data;

        HWND m_HWnd;

        static LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    };
}

#include <FlashlightEngine/Core/Window.inl>

#endif // FL_CORE_WINDOW_HPP
