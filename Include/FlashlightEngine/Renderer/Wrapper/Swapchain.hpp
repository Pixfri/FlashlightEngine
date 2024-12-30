// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#ifndef FL_RENDERER_WRAPPER_SWAPCHAIN_HPP
#define FL_RENDERER_WRAPPER_SWAPCHAIN_HPP

#include <FlashlightEngine/Types.hpp>

#include <FlashlightEngine/Renderer/Wrapper/Device.hpp>

namespace FlashlightEngine {
    class Swapchain {
    public:
        struct SwapchainInfo {
            VkExtent2D Extent;
            bool EnableVSync;
            VkSurfaceKHR Surface;
            std::shared_ptr<Swapchain> OldSwapchain = nullptr;
        };

        Swapchain(const std::shared_ptr<Device>& device, SwapchainInfo info);
        ~Swapchain();

        Swapchain(const Swapchain&) = delete;
        Swapchain(Swapchain&& other) noexcept;

        [[nodiscard]] inline VkSwapchainKHR GetSwapchain() const;
        [[nodiscard]] inline std::vector<VkImage> GetImages() const;
        [[nodiscard]] inline UInt64 GetImageCount() const;
        [[nodiscard]] inline std::vector<VkImageView> GetImageViews() const;
        [[nodiscard]] inline VkImageView GetImageView(UInt32 index) const;
        [[nodiscard]] inline VkFormat GetImageFormat() const;
        [[nodiscard]] inline VkExtent2D GetExtent() const;
        [[nodiscard]] inline bool IsValid() const;

        Swapchain& operator=(const Swapchain&) = delete;
        Swapchain& operator=(Swapchain&& other) noexcept;
    
    private:
        VkSwapchainKHR m_Swapchain{VK_NULL_HANDLE};

        std::vector<VkImage> m_SwapchainImages;
        std::vector<VkImageView> m_SwapchainImageViews;
        VkFormat m_SwapchainImageFormat{};
        VkExtent2D m_SwapchainExtent{};
        SwapchainInfo m_SwapchainInfo{};

        std::shared_ptr<Device> m_Device{nullptr};

        void CreateSwapchain();
        void CreateSwapchainImageViews();

        // Utility functions
        static VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) const;
        VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) const;
    };
}

#include <FlashlightEngine/Renderer/Wrapper/Swapchain.inl>

#endif // FL_RENDERER_WRAPPER_SWAPCHAIN_HPP
