// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#ifndef FL_RENDERER_WRAPPER_DEVICE_HPP
#define FL_RENDERER_WRAPPER_DEVICE_HPP

#include <FlashlightEngine/Types.hpp>

#include <FlashlightEngine/Renderer/Wrapper/Instance.hpp>
#include <FlashlightEngine/Renderer/Wrapper/Surface.hpp>

namespace FlashlightEngine {
    struct QueueFamilyIndices {
        UInt32 GraphicsFamily;
        UInt32 PresentFamily;
        UInt32 TransferFamily;
        UInt32 ComputeFamily;
        bool GraphicsFamilyFound;
        bool PresentFamilyFound;
        bool TransferFamilyFound;
        bool ComputeFamilyFound;

        [[nodiscard]] bool IsComplete() const {
            return GraphicsFamilyFound && PresentFamilyFound && TransferFamilyFound && ComputeFamilyFound;
        }
    };

    struct SwapchainSupportDetails {
        VkSurfaceCapabilitiesKHR Capabilities;
        std::vector<VkSurfaceFormatKHR> Formats;
        std::vector<VkPresentModeKHR> PresentModes;
    };

    class Device {
    public:
        Device(const Instance& instance, const std::shared_ptr<Surface>& surface, bool enableValidationLayers);
        ~Device();

        Device(const Device&) = delete;
        Device(Device&& other) noexcept;

        [[nodiscard]] inline VkPhysicalDevice GetPhysicalDevice() const;
        [[nodiscard]] inline VkPhysicalDeviceProperties GetPhysicalDeviceProperties() const;
        [[nodiscard]] inline QueueFamilyIndices GetQueueFamilies() const;
        [[nodiscard]] inline SwapchainSupportDetails GetSwapchainSupport() const;
        [[nodiscard]] inline VkDevice GetDevice() const;
        [[nodiscard]] inline VkQueue GetGraphicsQueue() const;
        [[nodiscard]] inline VkQueue GetPresentQueue() const;
        [[nodiscard]] inline VkQueue GetTransferQueue() const;
        [[nodiscard]] inline VkQueue GetComputeQueue() const;
        [[nodiscard]] inline bool IsValid() const;

        Device& operator=(const Device&) = delete;
        Device& operator=(Device&& other) noexcept;
    
    private:
        VkPhysicalDevice m_PhysicalDevice{VK_NULL_HANDLE};

        VkDevice m_Device{VK_NULL_HANDLE};
        VkQueue m_GraphicsQueue{VK_NULL_HANDLE};
        VkQueue m_PresentQueue{VK_NULL_HANDLE};
        VkQueue m_TransferQueue{VK_NULL_HANDLE};
        VkQueue m_ComputeQueue{VK_NULL_HANDLE};

        std::shared_ptr<Surface> m_Surface{nullptr};

        void PickPhysicalDevice(const Instance& instance);
        void CreateDevice(const Instance& instance, bool enableValidationLayers);

        // Utility functions
        bool IsDeviceSuitable(VkPhysicalDevice device) const;
        QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device) const;
        SwapchainSupportDetails QuerySwapchainSupport(VkPhysicalDevice device) const;
        bool CheckExtensionsSupport(VkPhysicalDevice device) const;

        // Constants
        std::vector<const char*> m_RequiredExtensions = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };
    };
}

#include <FlashlightEngine/Renderer/Wrapper/Device.inl>

#endif // FL_RENDERER_WRAPPER_DEVICE_HPP
