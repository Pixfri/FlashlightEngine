// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#ifndef FL_VULKANRHI_DEVICE_HPP
#define FL_VULKANRHI_DEVICE_HPP

#include <FlashlightEngine/Types.hpp>

#include <FlashlightEngine/VulkanRHI/Instance.hpp>
#include <FlashlightEngine/VulkanRHI/Surface.hpp>

#include <memory>
#include <vector>

#include <vk_mem_alloc.h>

namespace FlashlightEngine::Vk {
    struct SwapchainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities{};
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    struct QueueFamilyIndices {
        UInt32 GraphicsFamily{};
        UInt32 PresentFamily{};
        UInt32 TransferFamily{};
        UInt32 ComputeFamily{};
        bool GraphicsFamilyFound = false;
        bool PresentFamilyFound = false;
        bool TransferFamilyFound = false;
        bool ComputeFamilyFound = false;

        [[nodiscard]] bool IsComplete() const {
            return GraphicsFamilyFound && PresentFamilyFound && TransferFamilyFound && ComputeFamilyFound;
        }
    };

    class Device {
    public:
        explicit Device(const std::shared_ptr<Instance>& instance, const std::shared_ptr<Surface>& surface);
        ~Device();

        Device(const Device&) = delete;
        Device(Device&&) = delete;

        [[nodiscard]] inline VkPhysicalDevice GetPhysicalDevice() const;
        [[nodiscard]] inline VkDevice GetDevice() const;
        [[nodiscard]] inline VkQueue GetGraphicsQueue() const;
        [[nodiscard]] inline VkQueue GetPresentQueue() const;
        [[nodiscard]] inline VkQueue GetTransferQueue() const;
        [[nodiscard]] inline VkQueue GetComputeQueue() const;

        [[nodiscard]] inline VmaAllocator GetAllocator() const;

        [[nodiscard]] inline SwapchainSupportDetails GetSwapchainSupport() const;
        [[nodiscard]] inline QueueFamilyIndices GetQueueIndices() const;
        [[nodiscard]] inline VkPhysicalDeviceProperties GetPhysicalDeviceProperties() const;

        inline void WaitIdle() const;

        Device& operator=(const Device&) = delete;
        Device& operator=(Device&&) = delete;

    private:
        std::shared_ptr<Instance> m_Instance;
        std::shared_ptr<Surface> m_Surface;

        void PickPhysicalDevice();
        void CreateDevice();
        void CreateAllocator();

        bool IsDeviceSuitable(VkPhysicalDevice device) const;
        QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device) const;
        bool CheckDeviceExtensionSupport(VkPhysicalDevice device) const;
        SwapchainSupportDetails QuerySwapchainSupport(VkPhysicalDevice device) const;

        VkPhysicalDevice m_PhysicalDevice{VK_NULL_HANDLE};

        const std::vector<const char*> m_DeviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

        VkDevice m_Device{VK_NULL_HANDLE};
        VkQueue m_GraphicsQueue{VK_NULL_HANDLE};
        VkQueue m_PresentQueue{VK_NULL_HANDLE};
        VkQueue m_TransferQueue{VK_NULL_HANDLE};
        VkQueue m_ComputeQueue{VK_NULL_HANDLE};

        VmaAllocator m_Allocator{VK_NULL_HANDLE};
    };
}

#include <FlashlightEngine/VulkanRHI/Device.inl>

#endif // FL_VULKANRHI_DEVICE_HPP
