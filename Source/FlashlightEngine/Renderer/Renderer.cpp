// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#include <FlashlightEngine/Renderer/Renderer.hpp>

namespace FlashlightEngine {
    Renderer::Renderer(const std::shared_ptr<Window>& window)
        : m_Window(window) {
        m_Device = std::make_shared<Device>();
        m_Swapchain = std::make_shared<Swapchain>(m_Window, m_Device);

        // Set the default primitive topology to be a triangle list.
        SetPrimitiveTopology(PrimitiveTopology::TriangleList);
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
        if (const HRESULT hr = m_Swapchain->GetSwapchain()->Present(m_Window->IsVSync() ? 1 : 0, 0); FAILED(hr)) {
            spdlog::error("[DirectX] Failed to present swapchain. Error: {}", HResultToString(hr));
            if (hr == DXGI_ERROR_DEVICE_REMOVED) {
                spdlog::error("[DirectX] Device removed. Reason: {}",
                              HResultToString(m_Device->GetDevice()->GetDeviceRemovedReason()));
            }
        }
    }

    void Renderer::UseShaderCollection(const ShaderCollection& collection) const {
        collection.ApplyToContext(*m_Device);
    }

    void Renderer::BindVertexBuffer(const Buffer& buffer, const VertexType vertexType, const UInt32 offset) const {
        const UInt32 stride = ShaderCollection::GetLayoutByteSize(vertexType);
        m_Device->GetDeviceContext()->IASetVertexBuffers(0, 1, buffer.GetBuffer().GetAddressOf(), &stride, &offset);
    }

    void Renderer::SetPrimitiveTopology(const PrimitiveTopology topology) const {
        m_Device->GetDeviceContext()->IASetPrimitiveTopology(
            static_cast<D3D11_PRIMITIVE_TOPOLOGY>(EnumAsInteger(topology))
        );
    }

    void Renderer::Draw(const UInt32 vertexCount, const UInt32 firstVertex) const {
        m_Device->GetDeviceContext()->Draw(vertexCount, firstVertex);
    }

    ShaderCollection Renderer::CreateShaderCollection(const VertexType vertexType,
                                                      const std::filesystem::path& vertexShaderPath,
                                                      const std::filesystem::path& pixelShaderPath) const {
        ShaderCollectionDescriptor desc{};
        desc.VertexType = vertexType;
        desc.VertexShaderPath = vertexShaderPath;
        desc.PixelShaderPath = pixelShaderPath;

        ShaderCollection collection = ShaderCollection::CreateShaderCollection(desc, *m_Device);

        return collection;
    }

    std::unique_ptr<Buffer> Renderer::CreateBuffer(const void* data,
                                                   const UInt32 size,
                                                   const D3D11_USAGE usage,
                                                   const D3D11_BIND_FLAG bindFlag,
                                                   const bool hasCpuAccess,
                                                   const D3D11_CPU_ACCESS_FLAG cpuAccess) const {
        auto buffer = std::make_unique<Buffer>(m_Device, data, size, usage, bindFlag, hasCpuAccess, cpuAccess);

        return buffer;
    }

    std::shared_ptr<Sampler> Renderer::CreateSampler(const D3D11_FILTER filter,
                                                     const D3D11_TEXTURE_ADDRESS_MODE addressModeU,
                                                     const D3D11_TEXTURE_ADDRESS_MODE addressModeV,
                                                     const D3D11_TEXTURE_ADDRESS_MODE addressModeW) const {
        return std::make_shared<Sampler>(m_Device, filter, addressModeU, addressModeV, addressModeW);
    }

    std::unique_ptr<Texture> Renderer::CreateTexture(const std::filesystem::path& path) const {
        return std::make_unique<Texture>(path, m_Device);
    }
}
