// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#ifndef FL_VULKANRHI_INSTANCE_HPP
#define FL_VULKANRHI_INSTANCE_HPP

#include <volk.h>

#include <vector>

namespace FlashlightEngine::Vk {
    class Instance {
    public:
        Instance();
        ~Instance();

        Instance(const Instance&) = delete;
        Instance(Instance&&) = delete;

        [[nodiscard]] inline VkInstance GetHandle() const;
#if defined(FL_DEBUG) || defined(FL_FORCE_VALIDATION_LAYERS)
        [[nodiscard]] inline std::vector<const char*> GetEnabledLayers() const;
#endif

        Instance& operator=(const Instance&) = delete;
        Instance& operator=(Instance&&) = delete;

    private:
        void CreateInstance();
#if defined(FL_DEBUG) || defined(FL_FORCE_VALIDATION_LAYERS)
        void SetupDebugMessenger();
#endif

        // ============== Helper functions ============== //
#if defined(FL_DEBUG) || defined(FL_FORCE_VALIDATION_LAYERS)
        void CheckValidationLayerSupport() const;
        static void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
#endif

        [[nodiscard]] static std::vector<const char*> GetRequiredExtensions();

        static void CheckWSIExtensionSupport();

        // ============== Vulkan handles ============== //
        VkInstance m_Instance{VK_NULL_HANDLE};

#if defined(FL_DEBUG) || defined(FL_FORCE_VALIDATION_LAYERS)
        VkDebugUtilsMessengerEXT m_DebugMessenger{VK_NULL_HANDLE};
        const std::vector<const char*> m_ValidationLayers = {"VK_LAYER_KHRONOS_validation"};
#endif
    };
}

#include <FlashlightEngine/VulkanRHI/Instance.inl>

#endif // FL_VULKANRHI_INSTANCE_HPP
