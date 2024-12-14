// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#include <FlashlightEngine/Types.hpp>

#include <FlashlightEngine/VulkanRHI/Instance.hpp>

#include <FlashlightEngine/Core/Logger.hpp>

#include <GLFW/glfw3.h>

#include <unordered_set>

namespace FlashlightEngine::Vk {
    namespace {
        VKAPI_ATTR VkBool32 VKAPI_CALL DebugMessengerCallback(
            const VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
            const VkDebugUtilsMessageTypeFlagsEXT messageTypes,
            const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
            void* /*pUserData*/) {
            std::stringstream ss;

            ss << "[Vulkan Validation Layers]";

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

            if (messageTypes & VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT) {
                ss << "[General]";
            }
            if (messageTypes & VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT) {
                ss << "[Validation]";
            }
            if (messageTypes & VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT) {
                ss << "[Performance]";
            }

            ss << "[" << pCallbackData->messageIdNumber;
            if (pCallbackData->pMessageIdName) {
                ss << ":" << pCallbackData->pMessageIdName;
            }

            ss << "]: " << pCallbackData->pMessage;

            if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
                Log::Error(ss.str());
            } else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
                Log::Warn(ss.str());
            } else {
                Log::Info(ss.str());
            }

            return VK_FALSE; // Do not abort the call.
        }
    }

    Instance::Instance() {
        if (volkInitialize() != VK_SUCCESS) {
            Log::Error("[Vulkan] Failed to load Vulkan!");
            throw std::runtime_error("Failed to load Vulkan!");
        }

        CreateInstance();

#if defined(FL_DEBUG) || defined(FL_FORCE_VALIDATION_LAYERS)
        SetupDebugMessenger();
#endif
    }

    Instance::~Instance() {
#if defined(FL_DEBUG) || defined(FL_FORCE_VALIDATION_LAYERS)
        if (m_DebugMessenger != VK_NULL_HANDLE) {
            vkDestroyDebugUtilsMessengerEXT(m_Instance, m_DebugMessenger, nullptr);
        }
#endif

        if (m_Instance != VK_NULL_HANDLE) {
            vkDestroyInstance(m_Instance, nullptr);
        }
    }

    void Instance::CreateInstance() {
#if defined(FL_DEBUG) || defined(FL_FORCE_VALIDATION_LAYERS)
        CheckValidationLayerSupport();
#endif

        VkApplicationInfo appInfo = {};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "FlashlightEngine";
        appInfo.applicationVersion = VK_MAKE_API_VERSION(1, 0, 0, 0);
        appInfo.pEngineName = "FlashlightEngine";
        appInfo.engineVersion = VK_MAKE_API_VERSION(1, 0, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_3;

        VkInstanceCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        const auto extensions = GetRequiredExtensions();
        createInfo.enabledExtensionCount = static_cast<UInt32>(extensions.size());
        createInfo.ppEnabledExtensionNames = extensions.data();

#if defined(FL_DEBUG) || defined(FL_FORCE_VALIDATION_LAYERS)
        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo = {};
        createInfo.enabledLayerCount = static_cast<UInt32>(m_ValidationLayers.size());
        createInfo.ppEnabledLayerNames = m_ValidationLayers.data();

        PopulateDebugMessengerCreateInfo(debugCreateInfo);
        createInfo.pNext = &debugCreateInfo;
#endif

        if (vkCreateInstance(&createInfo, nullptr, &m_Instance) != VK_SUCCESS) {
            Log::Error("[Vulkan] Failed to create Vulkan instance!");
            throw std::runtime_error("Failed to create Vulkan instance!");
        }

        volkLoadInstance(m_Instance);

        CheckWSIExtensionSupport();
    }

#if defined(FL_DEBUG) || defined(FL_FORCE_VALIDATION_LAYERS)
    void Instance::SetupDebugMessenger() {
        VkDebugUtilsMessengerCreateInfoEXT createInfo;
        PopulateDebugMessengerCreateInfo(createInfo);
        if (vkCreateDebugUtilsMessengerEXT(m_Instance, &createInfo, nullptr, &m_DebugMessenger) != VK_SUCCESS) {
            Log::Error("[Vulkan] Failed to set up debug messenger!");
            throw std::runtime_error("Failed to set up debug messenger!");
        }
    }
#endif


#if defined(FL_DEBUG) || defined(FL_FORCE_VALIDATION_LAYERS)
    void Instance::CheckValidationLayerSupport() const {
        UInt32 layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        for (const char* layerName : m_ValidationLayers) {
            bool layerFound = false;

            for (const auto& layerProperties : availableLayers) {
                if (strcmp(layerName, layerProperties.layerName) == 0) {
                    layerFound = true;
                    break;
                }
            }

            if (!layerFound) {
                Log::Error("[Vulkan] Validation layer \"{}\" not found!", layerName);
                throw std::runtime_error("Validation layer not found!");
            }
        }
    }

    void Instance::PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
        createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity =
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        createInfo.messageType =
            VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfo.pfnUserCallback = DebugMessengerCallback;
        createInfo.pUserData = nullptr;
    }
#endif


    std::vector<const char*> Instance::GetRequiredExtensions() {
        UInt32 glfwExtensionCount = 0;
        const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        std::vector extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

#if defined(FL_DEBUG) || defined(FL_FORCE_VALIDATION_LAYERS)
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif

        return extensions;
    }

    void Instance::CheckWSIExtensionSupport() {
        UInt32 extensionCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, availableExtensions.data());

        Log::Info("[Vulkan] Available instance extensions:");
        std::unordered_set<std::string> available;
        for (const auto& [extensionName, specVersion] : availableExtensions) {
            Log::Info("\t- {}", extensionName);
            available.insert(extensionName);
        }

        Log::Info("[Vulkan] Required instance extensions:");
        for (const auto required = GetRequiredExtensions();
             const auto& extension : required) {
            Log::Info("\t- {}", extension);

            if (!available.contains(extension)) {
                Log::Error("[Vulkan] Extension \"{}\" not found!", extension);
                throw std::runtime_error("Extension not found!");
            }
        }
    }
}
