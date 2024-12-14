// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#include <FlashlightEngine/Core/Window.hpp>

#include <FlashlightEngine/Core/Logger.hpp>

#include <imgui_impl_win32.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace FlashlightEngine {
    Window::Window(const WindowProperties& properties)
        : m_Data(properties.Width, properties.Height, properties.Title), m_HWnd(nullptr) {
        WNDCLASSA windowClass{};
        windowClass.lpszClassName = "FlashlightEngine";
        windowClass.lpfnWndProc = WindowProc;
        windowClass.hInstance = GetModuleHandle(nullptr);
        RegisterClassA(&windowClass);

        RECT windowRect = {0, 0, static_cast<LONG>(properties.Width), static_cast<LONG>(properties.Height)};
        AdjustWindowRect(&windowRect, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, FALSE);

        m_HWnd = CreateWindowA(
            windowClass.lpszClassName,
            properties.Title.c_str(),
            WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            windowRect.right,
            windowRect.bottom,
            nullptr, // We have no parent window.
            nullptr, // We aren't using menus.
            windowClass.hInstance,
            nullptr
        );

        if (!m_HWnd) {
            Log::Critical("Failed to create window!");
            return;
        }

        SetWindowLongPtr(m_HWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(&m_Data));

        m_Data.IsOpen = true;
        ShowWindow(m_HWnd, SW_SHOW);

        Log::Info("Window '{0}' created with size ({1}, {2})",
                  properties.Title,
                  properties.Width,
                  properties.Height
        );
    }

    Window::~Window() {
        if (m_HWnd) {
            DestroyWindow(m_HWnd);
        }
    }

    void Window::Update() const {
        // Retrieve OS messages
        MSG msg = {};
        while (PeekMessage(&msg, m_HWnd, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    LRESULT CALLBACK Window::WindowProc(HWND hWnd, const UINT msg, const WPARAM wParam, const LPARAM lParam) {
        auto* data = reinterpret_cast<WindowData*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

        if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam)) {
            return 1;
        }

        switch (msg) {
        case WM_CLOSE:
            {
                data->IsOpen = false;
                break;
            }
        case WM_SIZE:
            {
                const Int32 width = LOWORD(lParam), height = HIWORD(lParam);
                data->Width = static_cast<UInt32>(width);
                data->Height = static_cast<UInt32>(height);

                if (data->OnResizeCallback) {
                    data->OnResizeCallback(width, height);
                }
                break;
            }
        default:
            return DefWindowProc(hWnd, msg, wParam, lParam);
        }

        return 0;
    }
}
