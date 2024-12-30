// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

namespace FlashlightEngine {
    inline VkSwapchainKHR Swapchain::GetSwapchain() const {
        return m_Swapchain;
    }

    inline std::vector<VkImage> Swapchain::GetImages() const {
        return m_SwapchainImages;
    }

    inline UInt64 Swapchain::GetImageCount() const {
        return m_SwapchainImages.size();
    }

    inline std::vector<VkImageView> Swapchain::GetImageViews() const {
        return m_SwapchainImageViews;
    }

    inline VkImageView Swapchain::GetImageView(const UInt32 index) const {
        return m_SwapchainImageViews[index];
    }

    inline VkFormat Swapchain::GetImageFormat() const {
        return m_SwapchainImageFormat;
    }

    inline VkExtent2D Swapchain::GetExtent() const {
        return m_SwapchainExtent;
    }

    inline bool Swapchain::IsValid() const {
        return m_Swapchain != VK_NULL_HANDLE;
    }
}
