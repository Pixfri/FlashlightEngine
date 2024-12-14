// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#ifndef FL_CORE_WINDOW_HPP
#define FL_CORE_WINDOW_HPP

#include <FlashlightEngine/Types.hpp>

#include <GLFW/glfw3.h>

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

        [[nodiscard]] inline GLFWwindow* GetHandle() const;
        [[nodiscard]] inline UInt32 GetWidth() const;
        [[nodiscard]] inline UInt32 GetHeight() const;
        inline void GetSize(UInt32& width, UInt32& height) const;
        [[nodiscard]] inline std::string GetTitle() const;
        [[nodiscard]] inline bool IsOpen() const;

        void Update();
        void Close() const;
        inline void OnResize(const std::function<void(UInt32, UInt32)>& callback);

        Window& operator=(const Window&) = delete;
        Window& operator=(Window&&) = delete;
    
    private:
        struct WindowData {
            UInt32 Width, Height;
            std::string Title;
            std::function<void(UInt32, UInt32)> OnResizeCallback;
        } m_Data;

        GLFWwindow* m_Window;
    };
}

#include <FlashlightEngine/Core/Window.inl>

#endif // FL_CORE_WINDOW_HPP
