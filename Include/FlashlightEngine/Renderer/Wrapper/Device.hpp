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

    class Device {
    public:
        Device(const Instance& instance, const std::shared_ptr<Surface>& surface);
        ~Device();

        Device(const Device&) = delete;
        Device(Device&& other) noexcept;

        [[nodiscard]] inline VkPhysicalDevice GetPhysicalDevice() const;
        [[nodiscard]] inline VkPhysicalDeviceProperties GetPhysicalDeviceProperties() const;
        [[nodiscard]] inline QueueFamilyIndices GetQueueFamilies() const;
        [[nodiscard]] inline bool IsValid() const;

        Device& operator=(const Device&) = delete;
        Device& operator=(Device&& other) noexcept;
    
    private:
        VkPhysicalDevice m_PhysicalDevice{VK_NULL_HANDLE};

        std::shared_ptr<Surface> m_Surface{nullptr};

        void PickPhysicalDevice(const Instance& instance);

        // Utility functions
        bool IsDeviceSuitable(VkPhysicalDevice device) const;
        QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device) const;
    };
}

#include <FlashlightEngine/Renderer/Wrapper/Device.inl>

#endif // FL_RENDERER_WRAPPER_DEVICE_HPP
