// Copyright (C) 2024 Jean "Pixfri" Letessier
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

namespace FlashlightEngine {
    inline VkInstance Instance::GetInstance() const {
        return m_Instance;
    }

    inline bool Instance::IsValid() const {
        return m_Instance != VK_NULL_HANDLE;
    }

    inline VkResult Instance::GetLastResult() const {
        return m_LastResult;
    }
}
