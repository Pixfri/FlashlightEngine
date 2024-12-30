// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#ifndef FL_RENDERER_RENDERER_HPP
#define FL_RENDERER_RENDERER_HPP

#include <FlashlightEngine/Types.hpp>

#include <FlashlightEngine/Renderer/Enums.hpp>
#include <FlashlightEngine/Renderer/Wrapper/Instance.hpp>
#include <FlashlightEngine/Renderer/Wrapper/Surface.hpp>
#include <FlashlightEngine/Renderer/Wrapper/Device.hpp>
#include <FlashlightEngine/Renderer/Wrapper/Swapchain.hpp>

#include <FlashlightEngine/Core/Window.hpp>

namespace FlashlightEngine {
    struct FrameData {
        VkSemaphore SwapchainSemaphore;
        VkSemaphore RenderSemaphore;
        VkFence RenderFence;

        VkCommandPool CommandPool;
        VkCommandBuffer MainCommandBuffer;
    };

    constexpr UInt32 g_FramesInFlight = 2;

    class Renderer {
    public:
        Renderer(const std::shared_ptr<Window>& window, RendererValidationLevel validationLevel);
        ~Renderer();

        Renderer(const Renderer&) = delete;
        Renderer(Renderer&&) noexcept = default;

        VkCommandBuffer BeginFrame();
        bool EndFrame();
        void BeginRendering(VkClearColorValue clearColor);
        void EndRendering();

        static void TransitionImage(VkCommandBuffer cmd, VkImage image, VkImageLayout srcLayout, VkImageLayout dstLayout);

        Renderer& operator=(const Renderer&) = delete;
        Renderer& operator=(Renderer&&) noexcept = default;

    private:
        std::shared_ptr<Window> m_Window;

        std::shared_ptr<Instance> m_Instance;
        std::shared_ptr<Surface> m_Surface;
        std::shared_ptr<Device> m_Device;
        std::shared_ptr<Swapchain> m_Swapchain;

        FrameData m_Frames[g_FramesInFlight];
        UInt64 m_FrameNumber{0};
        UInt32 m_CurrentImageIndex;

        enum class PassState {
            None,

            RenderPass,
            ComputePass
        };

        PassState m_PassState;

        inline FrameData& GetCurrentFrame();

        void InitCommandBuffers();
        void InitSyncObjects();

        void RecreateSwapchain();
    };
}

#include <FlashlightEngine/Renderer/Renderer.inl>

#endif // FL_RENDERER_RENDERER_HPP
