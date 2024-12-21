// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#include <FlashlightEngine/Renderer/Renderer.hpp>

namespace FlashlightEngine {
    Renderer::Renderer(const std::shared_ptr<Window>& window)
        : m_Window(window) {
        m_Device = std::make_shared<Device>();
        m_Swapchain = std::make_shared<Swapchain>(m_Window, m_Device);
    }

    void Renderer::OnResize(const UInt32 width, const UInt32 height) const {
        m_Device->GetDeviceContext()->Flush();

        m_Swapchain->OnResize(width, height);
    }

    void Renderer::BeginFrame() const {
        const ComPtr<ID3D11DeviceContext> deviceContext = m_Device->GetDeviceContext();

        D3D11_VIEWPORT viewport{};
        viewport.TopLeftX = 0;
        viewport.TopLeftY = 0;
        viewport.Width = static_cast<Float32>(m_Window->GetWidth());
        viewport.Height = static_cast<Float32>(m_Window->GetHeight());
        viewport.MinDepth = 0.0f;
        viewport.MaxDepth = 1.0f;

        deviceContext->ClearRenderTargetView(m_Swapchain->GetRTV().Get(), m_ClearColor);
        deviceContext->RSSetViewports(1, &viewport);
        deviceContext->OMSetRenderTargets(1, m_Swapchain->GetRTV().GetAddressOf(), nullptr);
    }

    void Renderer::EndFrame() const {
        if (const HRESULT hr = m_Swapchain->GetSwapchain()->Present(1, 0); FAILED(hr)) {
            spdlog::error("[DirectX] Failed to present swapchain. Error: {}", HResultToString(hr));
        }
    }

}
