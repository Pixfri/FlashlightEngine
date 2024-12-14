// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

namespace FlashlightEngine::Vk {
    inline VkInstance Instance::GetHandle() const {
        return m_Instance;
    }

#if defined(FL_DEBUG) || defined(FL_FORCE_VALIDATION_LAYERS)
    inline std::vector<const char*> Instance::GetEnabledLayers() const {
        return m_ValidationLayers;
    }
#endif
}