// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#ifndef FL_RENDERER_RENDERER_HPP
#define FL_RENDERER_RENDERER_HPP

#include <FlashlightEngine/Types.hpp>

#include <FlashlightEngine/Renderer/Enums.hpp>

#include <FlashlightEngine/Core/Window.hpp>

namespace FlashlightEngine {
    class Renderer {
    public:
        Renderer(const std::shared_ptr<Window>& window);
        ~Renderer();

        Renderer(const Renderer&) = delete;
        Renderer(Renderer&&) = delete;

        Renderer& operator=(const Renderer&) = delete;
        Renderer& operator=(Renderer&&) = delete;

    private:
        std::shared_ptr<Window> m_Window;
    };
}

#include <FlashlightEngine/Renderer/Renderer.inl>

#endif // FL_RENDERER_RENDERER_HPP
