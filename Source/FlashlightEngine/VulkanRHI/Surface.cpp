// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.


#include <FlashlightEngine/VulkanRHI/Surface.hpp>

#include <FlashlightEngine/Core/Logger.hpp>

namespace FlashlightEngine::Vk {
    Surface::Surface(const std::shared_ptr<Instance>& instance, const std::shared_ptr<Window>& window)
        : m_Instance(instance) {
        if (glfwCreateWindowSurface(instance->GetHandle(), window->GetHandle(), nullptr, &m_Surface) != VK_SUCCESS) {
            Log::Error("[Vulkan] Failed to create window surface!");
            throw std::runtime_error("Failed to create window surface!");
        }
    }

    Surface::~Surface() {
        if (m_Surface != VK_NULL_HANDLE) {
            vkDestroySurfaceKHR(m_Instance->GetHandle(), m_Surface, nullptr);
        }
    }
}
