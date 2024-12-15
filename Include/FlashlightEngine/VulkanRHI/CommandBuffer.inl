// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

namespace FlashlightEngine::Vk {
    inline VkCommandPool CommandPool::GetHandle() const {
        return m_CommandPool;
    }

    inline CommandPoolType CommandPool::GetType() const {
        return m_Type;
    }

    inline VkCommandBuffer CommandBuffer::GetHandle() const {
        return m_CommandBuffer;
    }
}
