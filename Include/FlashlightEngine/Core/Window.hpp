// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#ifndef FL_CORE_WINDOW_HPP
#define FL_CORE_WINDOW_HPP

#include <FlashlightEngine/Types.hpp>

#include <FlashlightEngine/Core/Events.hpp>

#include <GLFW/glfw3.h>

#include <filesystem>
#include <string>

namespace FlashlightEngine {
    struct WindowProperties {
        UInt32 Width, Height;
        std::string Title;
        bool Fullscreen;
        std::filesystem::path IconPath = "NoIcon";

        explicit WindowProperties(const UInt32 width = 1920,
                                         const UInt32 height = 1080,
                                         std::string title = "IsometricGame",
                                         const bool fullscreen = false)
            : Width(width), Height(height), Title(std::move(title)), Fullscreen(fullscreen) {
        }
    };

    struct VideoMode {
        union {
            Int32 X, W;
        };

        union {
            Int32 Y, H;
        };
    };

    class Window {
    public:
        explicit Window(const WindowProperties& properties);
        ~Window();

        void OnUpdate();
        [[nodiscard]] inline UInt32 GetWidth() const;
        [[nodiscard]] inline UInt32 GetHeight() const;
        inline void GetSize(UInt32& width, UInt32& height) const;
        [[nodiscard]] inline VideoMode GetVideoMode() const;
        [[nodiscard]] inline std::string GetTitle() const;

        inline void SetTitle(const std::string& title);

        inline void SetEventCallback(const std::function<void(Event&)>& callback);

        [[nodiscard]] inline bool IsFocused() const;

        void SetVSync(bool enabled);
        [[nodiscard]] inline bool IsVSync() const;

        inline void SwapchainInvalidated();
        [[nodiscard]] inline bool ShouldInvalidateSwapchain() const;

        void SetFullscreen(bool state);
        void UpdateFullscreenMode();
        [[nodiscard]] inline bool IsFullscreen() const;

        [[nodiscard]] inline GLFWwindow* GetNativeWindow() const;

    private:
        GLFWwindow* m_Window{nullptr};

        static bool m_IsGlfwInit;

        void Init(const WindowProperties& properties);
        void Quit() const;

        struct WindowData {
            UInt32 Width, Height;
            std::string Title;
            bool VSync = false;
            bool HasFocus = true;
            bool Fullscreen = false;
            bool ShouldInvalidateSwapchain = false;
            bool ShouldUpdateFullscreenMode = false;

            std::function<void(Event&)> EventCallback;
        } m_Data;
    };
}

#include <FlashlightEngine/Core/Window.inl>

#endif // FL_CORE_WINDOW_HPP
