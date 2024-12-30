// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#include <FlashlightEngine/Renderer/Wrapper/Swapchain.hpp>

#include <FlashlightEngine/Renderer/Renderer.hpp>
#include <FlashlightEngine/Renderer/Wrapper/VulkanUtils.hpp>

namespace FlashlightEngine {
    Swapchain::Swapchain(const std::shared_ptr<Device>& device, SwapchainInfo info)
        : m_SwapchainInfo(std::move(info)), m_Device(device) {
        CreateSwapchain();
        CreateSwapchainImageViews();

        m_SwapchainInfo.OldSwapchain = nullptr;
    }

    Swapchain::~Swapchain() {
        if (IsValid()) {
            for (const auto& imageView : m_SwapchainImageViews) {
                vkDestroyImageView(m_Device->GetDevice(), imageView, nullptr);
            }

            vkDestroySwapchainKHR(m_Device->GetDevice(), m_Swapchain, nullptr);
        }
    }

    Swapchain::Swapchain(Swapchain&& other) noexcept {
        m_Swapchain = other.m_Swapchain;
        other.m_Swapchain = VK_NULL_HANDLE;

        std::swap(m_SwapchainImages, other.m_SwapchainImages);
        std::swap(m_SwapchainImageViews, other.m_SwapchainImageViews);
        std::swap(m_SwapchainImageFormat, other.m_SwapchainImageFormat);
        std::swap(m_SwapchainExtent, other.m_SwapchainExtent);
        std::swap(m_Device, other.m_Device);
    }

    VkResult Swapchain::AcquireNextImage(const FrameData& frameData, UInt32& swapchainImageIndex) const {
        return vkAcquireNextImageKHR(m_Device->GetDevice(),
                                     m_Swapchain,
                                     1000000000,
                                     frameData.SwapchainSemaphore,
                                     nullptr,
                                     &swapchainImageIndex);
    }

    Swapchain& Swapchain::operator=(Swapchain&& other) noexcept {
        m_Swapchain = other.m_Swapchain;
        other.m_Swapchain = VK_NULL_HANDLE;

        std::swap(m_SwapchainImages, other.m_SwapchainImages);
        std::swap(m_SwapchainImageViews, other.m_SwapchainImageViews);
        std::swap(m_SwapchainImageFormat, other.m_SwapchainImageFormat);
        std::swap(m_SwapchainExtent, other.m_SwapchainExtent);
        std::swap(m_Device, other.m_Device);

        return *this;
    }

    VkResult Swapchain::Present(const FrameData& frameData, const UInt32 imageIndex) const {
        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.pSwapchains = &m_Swapchain;
        presentInfo.swapchainCount = 1;

        presentInfo.pWaitSemaphores = &frameData.RenderSemaphore;
        presentInfo.waitSemaphoreCount = 1;

        presentInfo.pImageIndices = &imageIndex;

        return vkQueuePresentKHR(m_Device->GetPresentQueue(), &presentInfo);
    }

    void Swapchain::CreateSwapchain() {
        const auto [Capabilities, Formats, PresentModes] = m_Device->GetSwapchainSupport();

        const auto [format, colorSpace] = ChooseSwapSurfaceFormat(Formats);
        const VkPresentModeKHR presentMode = ChooseSwapPresentMode(PresentModes);
        const VkExtent2D extent = ChooseSwapExtent(Capabilities);

        UInt32 imageCount = Capabilities.minImageCount + 1;
        if (Capabilities.maxImageCount > 0 &&
            imageCount > Capabilities.maxImageCount) {
            imageCount = Capabilities.maxImageCount;
        }

        VkSwapchainCreateInfoKHR swapchainInfo{};
        swapchainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;

        swapchainInfo.surface = m_SwapchainInfo.Surface;
        swapchainInfo.minImageCount = imageCount;
        swapchainInfo.imageFormat = format;
        swapchainInfo.imageColorSpace = colorSpace;
        swapchainInfo.imageExtent = extent;
        swapchainInfo.imageArrayLayers = 1;
        swapchainInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;

        const QueueFamilyIndices indices = m_Device->GetQueueFamilies();
        const UInt32 queueFamilyIndices[] = {indices.GraphicsFamily, indices.PresentFamily};

        swapchainInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        swapchainInfo.queueFamilyIndexCount = 0;
        swapchainInfo.pQueueFamilyIndices = nullptr;

        if (indices.GraphicsFamily != indices.PresentFamily) {
            swapchainInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            swapchainInfo.queueFamilyIndexCount = 2;
            swapchainInfo.pQueueFamilyIndices = queueFamilyIndices;
        }

        swapchainInfo.preTransform = Capabilities.currentTransform;
        swapchainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        swapchainInfo.presentMode = presentMode;
        swapchainInfo.clipped = VK_TRUE;

        swapchainInfo.oldSwapchain = m_SwapchainInfo.OldSwapchain == nullptr
                                         ? VK_NULL_HANDLE
                                         : m_SwapchainInfo.OldSwapchain->m_Swapchain;

        if (const VkResult result = vkCreateSwapchainKHR(m_Device->GetDevice(), &swapchainInfo, nullptr, &m_Swapchain);
            result != VK_SUCCESS) {
            spdlog::error("[Vulkan] Failed to create swapchain. Error: {}", VkResultToString(result));
            throw std::runtime_error("[Vulkan] Failed to create swapchain.");
        }

        vkGetSwapchainImagesKHR(m_Device->GetDevice(), m_Swapchain, &imageCount, nullptr);
        m_SwapchainImages.resize(imageCount);
        vkGetSwapchainImagesKHR(m_Device->GetDevice(), m_Swapchain, &imageCount, m_SwapchainImages.data());

        m_SwapchainImageFormat = format;
        m_SwapchainExtent = extent;
    }

    void Swapchain::CreateSwapchainImageViews() {
        m_SwapchainImageViews.resize(m_SwapchainImages.size());
        for (UInt64 i = 0; i < m_SwapchainImages.size(); i++) {
            VkImageViewCreateInfo viewInfo{};
            viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            viewInfo.image = m_SwapchainImages[i];
            viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            viewInfo.format = m_SwapchainImageFormat;
            viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            viewInfo.subresourceRange.baseMipLevel = 0;
            viewInfo.subresourceRange.levelCount = 1;
            viewInfo.subresourceRange.baseArrayLayer = 0;
            viewInfo.subresourceRange.layerCount = 1;

            if (const VkResult result = vkCreateImageView(m_Device->GetDevice(), &viewInfo, nullptr,
                                                          &m_SwapchainImageViews[i]);
                result != VK_SUCCESS) {
                spdlog::error("[Vulkan] Failed to create swapchain image views. Error: {}", VkResultToString(result));
                throw std::runtime_error("[Vulkan] Failed to create swapchain image views.");
            }
        }
    }

    VkSurfaceFormatKHR Swapchain::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
        for (const auto& format : availableFormats) {
            if (format.format == VK_FORMAT_B8G8R8A8_SRGB && format.colorSpace ==
                VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                return format;
            }
        }

        return availableFormats[0];
    }

    VkPresentModeKHR
    Swapchain::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) const {
        if (!m_SwapchainInfo.EnableVSync) {
            for (const auto& presentMode : availablePresentModes) {
                if (presentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
                    return presentMode;
                }
            }
        }

        return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkExtent2D Swapchain::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) const {
        if (capabilities.currentExtent.width != std::numeric_limits<UInt32>::max()) {
            return capabilities.currentExtent;
        }

        VkExtent2D actualExtent = m_SwapchainInfo.Extent;
        actualExtent.width = std::max(capabilities.minImageExtent.width,
                                      std::min(capabilities.maxImageExtent.width, actualExtent.width));
        actualExtent.height = std::max(capabilities.minImageExtent.height,
                                       std::min(capabilities.maxImageExtent.height, actualExtent.height));

        return actualExtent;
    }
}
