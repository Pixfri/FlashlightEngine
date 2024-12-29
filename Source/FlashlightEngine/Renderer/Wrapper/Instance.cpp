// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#include <FlashlightEngine/Renderer/Wrapper/Instance.hpp>

#include <FlashlightEngine/Renderer/Wrapper/VulkanUtils.hpp>

#include <FlashlightEngine/Core/CoreUtils.hpp>

#include <GLFW/glfw3.h>

#include <spdlog/spdlog.h>

#include <unordered_set>

namespace FlashlightEngine {
    Instance::Instance(const RendererValidationLevel validationLevel) {
        CreateInstance(validationLevel);
    }

    Instance::~Instance() {
        vkDestroyInstance(m_Instance, nullptr);

        spdlog::debug("[Vulkan] Instance destroyed.");
    }

    Instance::Instance(Instance&& other) noexcept {
        m_Instance = other.m_Instance;
        other.m_Instance = VK_NULL_HANDLE;

        m_LastResult = other.m_LastResult;
        other.m_LastResult = {};
    }

    Instance& Instance::operator=(Instance&& other) noexcept {
        m_Instance = other.m_Instance;
        other.m_Instance = VK_NULL_HANDLE;

        m_LastResult = other.m_LastResult;
        other.m_LastResult = {};

        return *this;
    }

    void Instance::CreateInstance(const RendererValidationLevel validationLevel) {
        m_LastResult = volkInitialize();

        FlAssert(m_LastResult == VK_SUCCESS, "[Vulkan] Failed to load the Vulkan library.");

        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;

        // These fields are used by the driver to do some optimizations on big engines/games, we don't really care about them.
        appInfo.pApplicationName = "Flashlight Engine Application";
        appInfo.applicationVersion = VK_MAKE_API_VERSION(1, 0, 0, 0);
        appInfo.pEngineName = "Flashlight Engine";
        appInfo.engineVersion = VK_MAKE_API_VERSION(1, 0, 0, 0);
        // We want to do some fancy stuff like dynamic rendering (Vulkan 1.3) and buffer device address (Vulkan 1.2).
        appInfo.apiVersion = VK_API_VERSION_1_3;

        VkInstanceCreateInfo instanceInfo{};
        instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;

        instanceInfo.pApplicationInfo = &appInfo;


        const auto requiredExtensions = GetRequiredExtensions();

        FlAssert(CheckExtensionsSupport(requiredExtensions), "At least one of the required Vulkan instance extension is not available.");

        instanceInfo.enabledExtensionCount = static_cast<UInt32>(requiredExtensions.size());
        instanceInfo.ppEnabledExtensionNames = requiredExtensions.data();

        instanceInfo.enabledLayerCount = 0;
        instanceInfo.ppEnabledLayerNames = nullptr;

        m_LastResult = vkCreateInstance(&instanceInfo, nullptr, &m_Instance);

        FlAssert(m_LastResult == VK_SUCCESS, "[Vulkan] Failed to create the Vulkan instance.");

        volkLoadInstance(m_Instance);

        spdlog::debug("[Vulkan] Successfully created and loaded instance.");
    }

    std::vector<const char*> Instance::GetRequiredExtensions() {
        UInt32 requiredWSIExtensionCount = 0;
        const char** requiredWSIExtensions = glfwGetRequiredInstanceExtensions(&requiredWSIExtensionCount);

        std::vector requiredExtensions(requiredWSIExtensions, requiredWSIExtensions + requiredWSIExtensionCount);

#if defined(FL_DEBUG) || defined(FL_FORCE_VULKAN_DEBUG)
        requiredExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif

        return requiredExtensions;
    }

    bool Instance::CheckExtensionsSupport(const std::vector<const char*>& requiredExtensions) {
        UInt32 availableExtensionCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &availableExtensionCount, nullptr);

        std::vector<VkExtensionProperties> availableExtensions(availableExtensionCount);
        vkEnumerateInstanceExtensionProperties(nullptr, &availableExtensionCount, availableExtensions.data());

        spdlog::debug("[Vulkan] Available instance extensions:");

        std::unordered_set<std::string> available;
        for (const auto& [extensionName, specVersion] : availableExtensions) {
            spdlog::debug("[Vulkan] \t- {}", extensionName);
            available.insert(extensionName);
        }

        spdlog::debug("[Vulkan] Required instance extensions:");
        for (const auto& required : requiredExtensions) {
            spdlog::debug("[Vulkan] \t-{}", required);
            if (!available.contains(required)) {
                spdlog::error("Required extensions \"{}\" is missing.", required);

                return false;
            }
        }

        return true;
    }
}
