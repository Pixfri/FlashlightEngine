// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#include <FlashlightEngine/Renderer/Renderer.hpp>

#include <FlashlightEngine/Renderer/Wrapper/VulkanUtils.hpp>

namespace FlashlightEngine {
    Renderer::Renderer(const std::shared_ptr<Window>& window, const RendererValidationLevel validationLevel)
        : m_Window(window) {
        window->SetTitle(window->GetTitle() + " <Vulkan 1.3>");

        m_Instance = std::make_shared<Instance>(validationLevel);

        m_Surface = std::make_shared<Surface>(m_Instance, *window);

        m_Device = std::make_shared<Device>(*m_Instance, m_Surface, validationLevel != RendererValidationLevel::None);

        Swapchain::SwapchainInfo swapchainInfo = {
            .Extent = window->GetExtent(),
            .EnableVSync = window->IsVSync(),
            .Surface = m_Surface->GetSurface(),
            .OldSwapchain = nullptr
        };

        m_Swapchain = std::make_shared<Swapchain>(m_Device, swapchainInfo);

        InitCommandBuffers();
        InitSyncObjects();
    }

    Renderer::~Renderer() {
        m_Device->WaitIdle();

        for (const auto& frame : m_Frames) {
            vkDestroyCommandPool(m_Device->GetDevice(), frame.CommandPool, nullptr);

            vkDestroyFence(m_Device->GetDevice(), frame.RenderFence, nullptr);
            vkDestroySemaphore(m_Device->GetDevice(), frame.SwapchainSemaphore, nullptr);
            vkDestroySemaphore(m_Device->GetDevice(), frame.RenderSemaphore, nullptr);
        }
    }

    VkCommandBuffer Renderer::BeginFrame() {
        VkResult result = VK_SUCCESS;

        result = vkWaitForFences(m_Device->GetDevice(), 1, &GetCurrentFrame().RenderFence, true, 1000000000);
        if (result != VK_SUCCESS) {
            spdlog::error("[Vulkan] Fence timeout. Error: {}", VkResultToString(result));
            return VK_NULL_HANDLE;
        }

        result = vkResetFences(m_Device->GetDevice(), 1, &GetCurrentFrame().RenderFence);
        if (result != VK_SUCCESS) {
            spdlog::error("[Vulkan] Failed to reset fence. Error: {}", VkResultToString(result));
            return VK_NULL_HANDLE;
        }

        result = m_Swapchain->AcquireNextImage(GetCurrentFrame(), m_CurrentImageIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR) {
            RecreateSwapchain();
            return VK_NULL_HANDLE;
        }
        if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
            spdlog::error("[Vulkan] Failed to acquire next swapchain image. Error: {}", VkResultToString(result));
            return VK_NULL_HANDLE;
        }

        const VkCommandBuffer cmd = GetCurrentFrame().MainCommandBuffer;

        result = vkResetCommandBuffer(cmd, 0);
        if (result != VK_SUCCESS) {
            spdlog::error("[Vulkan] Failed to reset command buffer. Error: {}", VkResultToString(result));
            return VK_NULL_HANDLE;
        }

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        result = vkBeginCommandBuffer(cmd, &beginInfo);
        if (result != VK_SUCCESS) {
            spdlog::error("[Vulkan] Failed to begin command recording. Error: {}", VkResultToString(result));
            return VK_NULL_HANDLE;
        }

        TransitionImage(cmd, m_Swapchain->GetImage(m_CurrentImageIndex), VK_IMAGE_LAYOUT_UNDEFINED,
                        VK_IMAGE_LAYOUT_GENERAL);

        return cmd;
    }

    bool Renderer::EndFrame() {
        VkResult result = VK_SUCCESS;

        const auto& frame = GetCurrentFrame();

        TransitionImage(frame.MainCommandBuffer, m_Swapchain->GetImage(m_CurrentImageIndex), VK_IMAGE_LAYOUT_GENERAL,
                        VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);

        result = vkEndCommandBuffer(frame.MainCommandBuffer);
        if (result != VK_SUCCESS) {
            spdlog::error("[Vulkan] Failed to record command buffer. Error: {}", VkResultToString(result));
            return false;
        }

        VkSemaphoreSubmitInfo waitInfo{};
        waitInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO;

        waitInfo.semaphore = frame.SwapchainSemaphore;
        waitInfo.stageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT;
        waitInfo.deviceIndex = 0;
        waitInfo.value = 1;

        VkSemaphoreSubmitInfo signalInfo{};
        signalInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO;

        signalInfo.semaphore = frame.RenderSemaphore;
        signalInfo.stageMask = VK_PIPELINE_STAGE_2_ALL_GRAPHICS_BIT;
        signalInfo.deviceIndex = 0;
        signalInfo.value = 1;

        VkCommandBufferSubmitInfo cmdSubmitInfo{};
        cmdSubmitInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO;
        cmdSubmitInfo.commandBuffer = frame.MainCommandBuffer;
        cmdSubmitInfo.deviceMask = 0;

        VkSubmitInfo2 submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO_2;

        submitInfo.waitSemaphoreInfoCount = 1;
        submitInfo.pWaitSemaphoreInfos = &waitInfo;

        submitInfo.signalSemaphoreInfoCount = 1;
        submitInfo.pSignalSemaphoreInfos = &signalInfo;

        submitInfo.commandBufferInfoCount = 1;
        submitInfo.pCommandBufferInfos = &cmdSubmitInfo;

        result = vkQueueSubmit2(m_Device->GetGraphicsQueue(), 1, &submitInfo, frame.RenderFence);
        if (result != VK_SUCCESS) {
            spdlog::error("[Vulkan] Failed to submit commands to the graphics queue. Error: {}",
                          VkResultToString(result));
            return false;
        }

        result = m_Swapchain->Present(GetCurrentFrame(), m_CurrentImageIndex);

        m_Window->UpdateFullscreenMode();

        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || m_Window->
            ShouldInvalidateSwapchain()) {
            RecreateSwapchain();
            m_Window->SwapchainInvalidated();
            return false;
        }

        if (result != VK_SUCCESS) {
            spdlog::error("[Vulkan] Failed to present image to swapchain. Error: {}", VkResultToString(result));
            return false;
        }

        m_FrameNumber++;

        return true;
    }

    void Renderer::BeginRendering(const VkClearColorValue clearColor) {
        const VkCommandBuffer cmd = GetCurrentFrame().MainCommandBuffer;

        VkImageSubresourceRange clearRange{};
        clearRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        clearRange.baseMipLevel = 0;
        clearRange.levelCount = VK_REMAINING_MIP_LEVELS;
        clearRange.baseArrayLayer = 0;
        clearRange.layerCount = VK_REMAINING_ARRAY_LAYERS;

        vkCmdClearColorImage(cmd, m_Swapchain->GetImage(m_CurrentImageIndex), VK_IMAGE_LAYOUT_GENERAL, &clearColor, 1,
                             &clearRange);

        TransitionImage(cmd, m_Swapchain->GetImage(m_CurrentImageIndex), VK_IMAGE_LAYOUT_GENERAL,
                        VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
    }

    void Renderer::EndRendering() {
        const VkCommandBuffer cmd = GetCurrentFrame().MainCommandBuffer;

        TransitionImage(cmd, m_Swapchain->GetImage(m_CurrentImageIndex), VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_GENERAL);
    }

    void Renderer::TransitionImage(const VkCommandBuffer cmd, const VkImage image, const VkImageLayout srcLayout,
                                   const VkImageLayout dstLayout) {
        VkImageMemoryBarrier2 imageBarrier{};
        imageBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2;

        imageBarrier.srcStageMask = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT;
        imageBarrier.srcAccessMask = VK_ACCESS_2_MEMORY_WRITE_BIT;
        imageBarrier.dstStageMask = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT;
        imageBarrier.dstAccessMask = VK_ACCESS_2_MEMORY_WRITE_BIT | VK_ACCESS_2_MEMORY_READ_BIT;

        imageBarrier.oldLayout = srcLayout;
        imageBarrier.newLayout = dstLayout;

        const VkImageAspectFlags aspectMask = (dstLayout == VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL)
                                                  ? VK_IMAGE_ASPECT_DEPTH_BIT
                                                  : VK_IMAGE_ASPECT_COLOR_BIT;

        VkImageSubresourceRange subresourceRange{};
        subresourceRange.aspectMask = aspectMask;
        subresourceRange.baseMipLevel = 0;
        subresourceRange.levelCount = VK_REMAINING_MIP_LEVELS;
        subresourceRange.baseArrayLayer = 0;
        subresourceRange.layerCount = VK_REMAINING_ARRAY_LAYERS;

        imageBarrier.subresourceRange = subresourceRange;
        imageBarrier.image = image;

        VkDependencyInfo depInfo{};
        depInfo.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO;

        depInfo.imageMemoryBarrierCount = 1;
        depInfo.pImageMemoryBarriers = &imageBarrier;

        vkCmdPipelineBarrier2(cmd, &depInfo);
    }

    void Renderer::InitCommandBuffers() {
        // Create a command pool for commands submitted to the graphics queue.
        // We also want the pool to allow for resetting of individual command buffers.
        VkCommandPoolCreateInfo commandPoolInfo{};
        commandPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        commandPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        commandPoolInfo.queueFamilyIndex = m_Device->GetQueueFamilies().GraphicsFamily;

        for (UInt32 i = 0; i < g_FramesInFlight; i++) {
            VkResult result = vkCreateCommandPool(m_Device->GetDevice(), &commandPoolInfo, nullptr,
                                                  &m_Frames[i].CommandPool);

            if (result != VK_SUCCESS) {
                spdlog::error("[Vulkan] Failed to create command pool. Error: {}", VkResultToString(result));
                throw std::runtime_error("[Vulkan] Failed to create command pool.");
            }

            // Allocate the default command buffer that we will use for rendering
            VkCommandBufferAllocateInfo cmdAllocInfo{};
            cmdAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
            cmdAllocInfo.commandPool = m_Frames[i].CommandPool;
            cmdAllocInfo.commandBufferCount = 1;
            cmdAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

            result = vkAllocateCommandBuffers(m_Device->GetDevice(), &cmdAllocInfo, &m_Frames[i].MainCommandBuffer);
            if (result != VK_SUCCESS) {
                spdlog::error("[Vulkan] Failed to allocate command buffers. Error: {}", VkResultToString(result));
                throw std::runtime_error("[Vulkan] Failed to allocate command buffers.");
            }
        }
    }

    void Renderer::InitSyncObjects() {
        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkResult result = VK_SUCCESS;
        for (UInt32 i = 0; i < g_FramesInFlight; i++) {
            result = vkCreateFence(m_Device->GetDevice(), &fenceInfo, nullptr, &m_Frames[i].RenderFence);
            if (result != VK_SUCCESS) {
                spdlog::error("[Vulkan] Failed to create render fence. Error: {}", VkResultToString(result));
                throw std::runtime_error("[Vulkan] Failed to create render fence.");
            }

            result = vkCreateSemaphore(m_Device->GetDevice(), &semaphoreInfo, nullptr, &m_Frames[i].SwapchainSemaphore);
            if (result != VK_SUCCESS) {
                spdlog::error("[Vulkan] Failed to create swapchain semaphore. Error: {}", VkResultToString(result));
                throw std::runtime_error("[Vulkan] Failed to create swapchain semaphore.");
            }

            result = vkCreateSemaphore(m_Device->GetDevice(), &semaphoreInfo, nullptr, &m_Frames[i].RenderSemaphore);
            if (result != VK_SUCCESS) {
                spdlog::error("[Vulkan] Failed to create render semaphore. Error: {}", VkResultToString(result));
                throw std::runtime_error("[Vulkan] Failed to create render semaphore.");
            }
        }
    }

    void Renderer::RecreateSwapchain() {
        auto extent = m_Window->GetExtent();
        while (extent.width == 0 || extent.height == 0) {
            extent = m_Window->GetExtent();
            glfwWaitEvents();
        }
        m_Device->WaitIdle();

        Swapchain::SwapchainInfo info{};
        info.Extent = extent;
        info.EnableVSync = m_Window->IsVSync();
        info.Surface = m_Surface->GetSurface();
        info.OldSwapchain = std::move(m_Swapchain);

        m_Swapchain = std::make_shared<Swapchain>(m_Device, info);
    }
}
