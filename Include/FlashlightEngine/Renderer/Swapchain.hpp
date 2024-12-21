// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#ifndef FL_RENDERER_SWAPCHAIN_HPP
#define FL_RENDERER_SWAPCHAIN_HPP

#include <FlashlightEngine/Types.hpp>

#include <FlashlightEngine/Renderer/DxUtils.hpp>
#include <FlashlightEngine/Renderer/Device.hpp>

#include <FlashlightEngine/Core/Window.hpp>


namespace FlashlightEngine {
    class Swapchain {
    public:
        Swapchain(const std::shared_ptr<Window>& window, const std::shared_ptr<Device>& device);
        ~Swapchain();

        Swapchain(const Swapchain&) = delete;
        Swapchain(Swapchain&& other) noexcept;

        [[nodiscard]] inline ComPtr<IDXGISwapChain1> GetSwapchain() const;
        [[nodiscard]] inline ComPtr<ID3D11RenderTargetView> GetRTV() const;

        void OnResize(UInt32 width, UInt32 height);

        Swapchain& operator=(const Swapchain&) = delete;
        Swapchain& operator=(Swapchain&& other) noexcept;
    
    private:
        ComPtr<IDXGISwapChain1> m_Swapchain{nullptr};
        ComPtr<ID3D11RenderTargetView> m_RenderTargetView{nullptr};

        std::shared_ptr<Window> m_Window{nullptr};
        std::shared_ptr<Device> m_Device{nullptr};

        bool CreateSwapchain(UInt32 width, UInt32 height);
        bool CreateSwapchainResources();
        void DestroySwapchainResources();
    };
}

#include <FlashlightEngine/Renderer/Swapchain.inl>

#endif // FL_RENDERER_SWAPCHAIN_HPP
