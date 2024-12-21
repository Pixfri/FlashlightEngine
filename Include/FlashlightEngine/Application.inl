// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

namespace FlashlightEngine {
    inline Application& Application::Get() {
        return *m_Instance;
    }

    inline Window& Application::GetWindow() const {
        return *m_Window;
    }

    inline void Application::Close() {
        m_Running = false;
    }
}
