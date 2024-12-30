// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#include <FlashlightEngine/Renderer/Wrapper/Instance.hpp>

#include <FlashlightEngine/Renderer/Wrapper/VulkanUtils.hpp>

#include <FlashlightEngine/Core/CoreUtils.hpp>

#include <debugbreak.h>

#include <GLFW/glfw3.h>

#include <spdlog/spdlog.h>

#include <cstring>
#include <unordered_set>

namespace FlashlightEngine {
#if defined(FL_DEBUG) || defined(FL_FORCE_VULKAN_DEBUG)
    namespace {
        VKAPI_ATTR VkBool32 VKAPI_CALL DebugMessengerCallback(
            const VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
            const VkDebugUtilsMessageTypeFlagsEXT messageTypes,
            const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
            void* /*pUserData*/) {
            std::stringstream ss;

            ss << "[Vulkan][Validation Layer]";

            // Message severity

            if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT) {
                ss << "[Verbose]";
            }

            if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT) {
                ss << "[Info]";
            }

            if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
                ss << "[Warning]";
            }

            if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
                ss << "[Error]";
            }

            // Message types

            if (messageTypes & VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT) {
                ss << "[General]";
            }

            if (messageTypes & VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT) {
                ss << "[Performance]";
            }

            if (messageTypes & VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT) {
                ss << "[Validation]";
            }

            // Message id

            ss << "[" << pCallbackData->messageIdNumber;
            if (pCallbackData->pMessageIdName) {
                ss << ":" << pCallbackData->pMessageIdName;
            }

            ss << "]: " << pCallbackData->pMessage;

            if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
                spdlog::error(ss.str());
                debug_break();
            } else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
                spdlog::warn(ss.str());
            } else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT) {
                spdlog::info(ss.str());
            } else {
                spdlog::debug(ss.str());
            }

            // We don't abort the call. This should always be VK_FALSE.
            return VK_FALSE;
        }
    }
#endif

    Instance::Instance(const RendererValidationLevel validationLevel) {
        CreateInstance(validationLevel);

#if defined(FL_DEBUG) || defined(FL_FORCE_VULKAN_DEBUG)
        CreateDebugMessenger(validationLevel);
#endif
    }

    Instance::~Instance() {
        if (IsValid()) {
#if defined(FL_DEBUG) || defined(FL_FORCE_VULKAN_DEBUG)
            vkDestroyDebugUtilsMessengerEXT(m_Instance, m_DebugMessenger, nullptr);

            spdlog::debug("[Vulkan] Debug messenger destroyed.");
#endif

            vkDestroyInstance(m_Instance, nullptr);

            spdlog::debug("[Vulkan] Instance destroyed.");
        }
    }

    Instance::Instance(Instance&& other) noexcept {
        m_Instance = other.m_Instance;
        other.m_Instance = VK_NULL_HANDLE;

#if defined(FL_DEBUG) || defined(FL_FORCE_VULKAN_DEBUG)
        m_DebugMessenger = other.m_DebugMessenger;
        other.m_DebugMessenger = nullptr;
#endif

        m_LastResult = other.m_LastResult;
        other.m_LastResult = {};
    }

    Instance& Instance::operator=(Instance&& other) noexcept {
        m_Instance = other.m_Instance;
        other.m_Instance = VK_NULL_HANDLE;

#if defined(FL_DEBUG) || defined(FL_FORCE_VULKAN_DEBUG)
        m_DebugMessenger = other.m_DebugMessenger;
        other.m_DebugMessenger = nullptr;
#endif

        m_LastResult = other.m_LastResult;
        other.m_LastResult = {};

        return *this;
    }

    void Instance::CreateInstance(const RendererValidationLevel validationLevel) {
        m_LastResult = volkInitialize();

        if (m_LastResult != VK_SUCCESS) {
            spdlog::error("[Vulkan] Failed to load the Vulkan library. Error: {}", VkResultToString(m_LastResult));
            throw std::runtime_error("Failed to load the Vulkan library.");
        }

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

        FlAssert(CheckExtensionsSupport(requiredExtensions),
                 "[Vulkan] At least one of the required Vulkan instance extension is not available.");

        instanceInfo.enabledExtensionCount = static_cast<UInt32>(requiredExtensions.size());
        instanceInfo.ppEnabledExtensionNames = requiredExtensions.data();

#if defined(FL_DEBUG) || defined(FL_FORCE_VULKAN_DEBUG)
        FlAssert(CheckValidationLayerSupport(), "[Vulkan] Validation layers aren't available.");

        instanceInfo.enabledLayerCount = static_cast<UInt32>(m_ValidationLayers.size());
        instanceInfo.ppEnabledLayerNames = m_ValidationLayers.data();

        VkDebugUtilsMessengerCreateInfoEXT debugInfo{};
        PopulateDebugMessengerCreateInfo(debugInfo, validationLevel);

        instanceInfo.pNext = &debugInfo;
#else
        instanceInfo.enabledLayerCount = 0;
        instanceInfo.ppEnabledLayerNames = nullptr;
#endif

        m_LastResult = vkCreateInstance(&instanceInfo, nullptr, &m_Instance);
        if (m_LastResult != VK_SUCCESS) {
            spdlog::error("[Vulkan] Failed to create the Vulkan instance. Error: {}", VkResultToString(m_LastResult));
            throw std::runtime_error("Failed to create Vulkan instance.");
        }

        volkLoadInstance(m_Instance);

        spdlog::debug("[Vulkan] Successfully created and loaded instance.");
    }

#if defined(FL_DEBUG) || defined(FL_FORCE_VULKAN_DEBUG)
    void Instance::CreateDebugMessenger(const RendererValidationLevel validationLevel) {
        VkDebugUtilsMessengerCreateInfoEXT createInfo{};
        PopulateDebugMessengerCreateInfo(createInfo, validationLevel);

        m_LastResult = vkCreateDebugUtilsMessengerEXT(m_Instance, &createInfo, nullptr, &m_DebugMessenger);

        if (m_LastResult != VK_SUCCESS) {
            spdlog::error("[Vulkan] Failed to create Vulkan debug messenger. Error: {}",
                          VkResultToString(m_LastResult));
            throw std::runtime_error("Failed to create Vulkan debug messenger.");
        }

        spdlog::debug("[Vulkan] Successfully created Vulkan debug messenger.");
    }
#endif

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
        if (!std::ranges::all_of(requiredExtensions, [&available](const char* required) {
            spdlog::debug("[Vulkan] \t- {}", required);
            if (!available.contains(required)) {
                spdlog::error("[Vulkan] Required instance extension \"{}\" is missing.", required);
                return false;
            }

            return true;
        })) {
            return false;
        }

        return true;
    }

    bool Instance::CheckValidationLayerSupport() const {
        UInt32 layerCount = 0;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        std::vector<VkLayerProperties> layers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, layers.data());

        spdlog::debug("[Vulkan] Available layers:");
        for (const auto& layerProperties : layers) {
            spdlog::debug("[Vulkan] \t- {}", layerProperties.layerName);
        }

        spdlog::debug("[Vulkan] Required layers:");
        for (const auto& layer : m_ValidationLayers) {
            spdlog::debug("[Vulkan] \t- {}", layer);
        }

        for (const auto& layer : m_ValidationLayers) {
            bool layerFound = false;

            for (const auto& layerProperties : layers) {
                if (strcmp(layer, layerProperties.layerName) == 0) {
                    layerFound = true;
                    break;
                }
            }

            if (!layerFound) {
                spdlog::error("[Vulkan] Validation layers requested but not available.");
                return false;
            }
        }

        return true;
    }

    void Instance::PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo,
                                                    const RendererValidationLevel validationLevel) {
        createInfo = {};

        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;

        createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        if (validationLevel >= RendererValidationLevel::Verbose) {
            createInfo.messageSeverity |= VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT;
        }

        if (validationLevel >= RendererValidationLevel::Infos) {
            createInfo.messageSeverity |= VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT;
        }
        if (validationLevel >= RendererValidationLevel::Warnings) {
            createInfo.messageSeverity |= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT;
        }

        createInfo.messageType =
            VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

        createInfo.pfnUserCallback = DebugMessengerCallback;
    }
}
