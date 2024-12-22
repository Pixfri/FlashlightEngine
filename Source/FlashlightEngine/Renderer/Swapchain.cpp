// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#include <FlashlightEngine/Renderer/Swapchain.hpp>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include <spdlog/spdlog.h>

namespace FlashlightEngine {
    Swapchain::Swapchain(const std::shared_ptr<Window>& window, const std::shared_ptr<Device>& device)
        : m_Window(window), m_Device(device) {
        if (!CreateSwapchain(window->GetWidth(), window->GetHeight())) {
            spdlog::error("[DirectX] Failed to create swapchain.");
        }

        if (!CreateSwapchainResources()) {
            spdlog::error("[DirectX] Failed to create swapchain resources.");
        }
    }

    Swapchain::~Swapchain() {
        m_RenderTargetView.Reset();
        m_Swapchain.Reset();

        spdlog::info("[DirectX] Swapchain destroyed.");
    }

    Swapchain::Swapchain(Swapchain&& other) noexcept {
        m_Swapchain.Swap(other.m_Swapchain);
        m_RenderTargetView.Swap(other.m_RenderTargetView);
    }

    Swapchain& Swapchain::operator=(Swapchain&& other) noexcept {
        m_Swapchain.Swap(other.m_Swapchain);
        m_RenderTargetView.Swap(other.m_RenderTargetView);

        return *this;
    }

    void Swapchain::OnResize(const UInt32 width, const UInt32 height) {
        DestroySwapchainResources();

        if (const auto hr = m_Swapchain->ResizeBuffers(0, width, height, DXGI_FORMAT_B8G8R8A8_UNORM, 0); FAILED(hr)) {
            spdlog::error("[DirectX] Failed to resize swapchain. Error: {}", HResultToString(hr));
        }


        CreateSwapchainResources();
    }

    bool Swapchain::CreateSwapchain(const UInt32 width, const UInt32 height) {
        m_Swapchain.Reset();

        HRESULT hr = S_OK;

        DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
        swapChainDesc.Width = width;
        swapChainDesc.Height = height;
        swapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
        swapChainDesc.SampleDesc.Count = 1;
        swapChainDesc.SampleDesc.Quality = 0;
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.BufferCount = 2;
        swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
        swapChainDesc.Flags = 0;

        DXGI_SWAP_CHAIN_FULLSCREEN_DESC fullscreenDesc{};
        fullscreenDesc.Windowed = true;

        hr = m_Device->GetDxgiFactory()->CreateSwapChainForHwnd(
            m_Device->GetDevice().Get(),
            glfwGetWin32Window(m_Window->GetNativeWindow()),
            &swapChainDesc,
            &fullscreenDesc,
            nullptr,
            &m_Swapchain
        );

        if (FAILED(hr)) {
            spdlog::error("[DirectX] Failed to create swapchain. Error: {}", HResultToString(hr));
            return false;
        }

#if defined(FL_DEBUG) || defined(FL_FORCE_DX_DEBUG_INTERFACE)
        hr = m_Swapchain->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof("Swapchain"), "Swapchain");

        if (FAILED(hr)) {
            spdlog::error("[DirectX] Failed to set swapchain name. Error: {}", HResultToString(hr));
        }
#endif

        spdlog::info("[DirectX] Swapchain created.");

        return true;
    }


    bool Swapchain::CreateSwapchainResources() {
        ComPtr<ID3D11Texture2D> backBuffer = nullptr;

        HRESULT hr = m_Swapchain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));

        if (FAILED(hr)) {
            spdlog::error("[DirectX] Failed to get swapchain back buffer. Error: {}", HResultToString(hr));
            return false;
        }

        hr = m_Device->GetDevice()->CreateRenderTargetView(backBuffer.Get(), nullptr, &m_RenderTargetView);

        if (FAILED(hr)) {
            spdlog::error("[DirectX] Failed to create render target. Error: {}", HResultToString(hr));
            return false;
        }

#if defined(FL_DEBUG) || defined(FL_FORCE_DX_DEBUG_INTERFACE)
        hr = backBuffer->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof("Swapchain backbuffer"), "Swapchain backbuffer");

        if (FAILED(hr)) {
            spdlog::error("[DirectX] Failed to set swapchain back buffer name. Error: {}", HResultToString(hr));
        }

        hr = m_RenderTargetView->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof("Swapchain RTV"), "Swapchain RTV");

        if (FAILED(hr)) {
            spdlog::error("[DirectX] Failed to set RTV name. Error: {}", HResultToString(hr));
        }
#endif

        spdlog::info("[DirectX] Render target created.");

        return true;
    }

    void Swapchain::DestroySwapchainResources() {
        m_RenderTargetView.Reset();
    }
}
