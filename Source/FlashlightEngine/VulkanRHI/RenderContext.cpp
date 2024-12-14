// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#include <FlashlightEngine/VulkanRHI/RenderContext.hpp>

namespace FlashlightEngine::Vk {
    RenderContext::RenderContext(const std::shared_ptr<Window>& window)
        : m_Window(window) {
        m_VulkanInstance = std::make_shared<Instance>();

        m_VulkanSurface = std::make_shared<Surface>(m_VulkanInstance, m_Window);

        m_VulkanDevice = std::make_shared<Device>(m_VulkanInstance, m_VulkanSurface);
    }

    RenderContext::~RenderContext() {
        m_VulkanDevice->WaitIdle();
    }
}
