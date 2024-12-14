// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#ifndef FL_APPLICATION_HPP
#define FL_APPLICATION_HPP

#include <FlashlightEngine/Types.hpp>

#include <FlashlightEngine/Core/Window.hpp>

#include <FlashlightEngine/VulkanRHI/RenderContext.hpp>

#include <memory>


namespace FlashlightEngine {
    class Application {
    public:
        Application(UInt32 width, UInt32 height);
        ~Application() = default;

        Application(const Application&) = delete;
        Application(Application&&) = delete;

        void Run() const;

        Application& operator=(const Application&) = delete;
        Application& operator=(Application&&) = delete;
    
    private:
        std::shared_ptr<Window> m_Window;
        std::shared_ptr<Vk::RenderContext> m_VulkanRenderContext;
    };
}

#include <FlashlightEngine/Application.inl>

#endif // FL_APPLICATION_HPP
