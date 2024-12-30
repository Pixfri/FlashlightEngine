// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#include <FlashlightEngine/Renderer/Wrapper/Surface.hpp>

#include <FlashlightEngine/Renderer/Wrapper/VulkanUtils.hpp>

#include <spdlog/spdlog.h>

namespace FlashlightEngine {
    Surface::Surface(const std::shared_ptr<Instance>& instance, const Window& window)
        : m_Instance(instance) {
        if (const VkResult result = glfwCreateWindowSurface(m_Instance->GetInstance(),
                                                            window.GetNativeWindow(),
                                                            nullptr,
                                                            &m_Surface);
            result != VK_SUCCESS) {
            spdlog::error("[Vulkan] Failed to create window surface. Error: {}", VkResultToString(result));
            throw std::runtime_error("Failed to create window surface.");
        }

        spdlog::debug("[Vulkan] Successfully create window surface.");
    }

    Surface::~Surface() {
        if (IsValid()) {
            vkDestroySurfaceKHR(m_Instance->GetInstance(), m_Surface, nullptr);

            spdlog::debug("[Vulkan] Window surface destroyed.");
        }
    }

    Surface::Surface(Surface&& other) noexcept {
        m_Surface = other.m_Surface;
        other.m_Surface = VK_NULL_HANDLE;

        m_Instance = other.m_Instance;
        other.m_Instance = nullptr;
    }

    Surface& Surface::operator=(Surface&& other) noexcept {
        m_Surface = other.m_Surface;
        other.m_Surface = VK_NULL_HANDLE;

        m_Instance = other.m_Instance;
        other.m_Instance = nullptr;

        return *this;
    }
}
