// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#ifndef FL_CORE_APPLICATION_HPP
#define FL_CORE_APPLICATION_HPP

#include <FlashlightEngine/Types.hpp>

#include <FlashlightEngine/Core/Window.hpp>

#include <FlashlightEngine/Renderer/Renderer.hpp>

#include <expected>
#include <memory>
#include <string>

namespace FlashlightEngine {
    class Application {
    public:
        Application(UInt32 width, UInt32 height, const std::string& title);
        virtual ~Application();

        void Run();
        virtual void OnEvent(Event& e) = 0;
        virtual void OnUpdate() = 0;
        virtual void OnRender() = 0;
        bool OnWindowClose(WindowCloseEvent& e);
        bool OnWindowResize(const WindowResizeEvent& e) const;

        inline static Application& Get();
        [[nodiscard]] inline Window& GetWindow() const;
        inline void Close();

    private:
        std::shared_ptr<Window> m_Window;

    protected:
        std::shared_ptr<Renderer> m_Renderer;

    private:
        bool m_Running = false;
        static Application* m_Instance;
    };
}

#include <FlashlightEngine/Application.inl>

#endif // FL_CORE_APPLICATION_HPP
