// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

namespace FlashlightEngine {
    inline VkSurfaceKHR Surface::GetSurface() const {
        return m_Surface;
    }

    inline bool Surface::IsValid() const {
        return m_Surface != VK_NULL_HANDLE;
    }
}
