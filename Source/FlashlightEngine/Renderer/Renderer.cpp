// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#include <FlashlightEngine/Renderer/Renderer.hpp>

#include <FlashlightEngine/Core/Filesystem.hpp>

namespace FlashlightEngine {
    Renderer::Renderer(const std::shared_ptr<Window>& window)
        : m_Window(window) {
        m_Device = std::make_shared<Device>();
        m_Swapchain = std::make_shared<Swapchain>(m_Window, m_Device);

        // Set the default primitive topology to be a triangle list.
        SetPrimitiveTopology(PrimitiveTopology::TriangleList);

        D3D11_RASTERIZER_DESC rasterizerDesc{};
        rasterizerDesc.CullMode = D3D11_CULL_NONE;
        rasterizerDesc.FillMode = D3D11_FILL_SOLID;

        HRESULT hr = m_Device->GetDevice()->CreateRasterizerState(&rasterizerDesc, m_RasterizerState.GetAddressOf());
        if (FAILED(hr)) {
            spdlog::error("[DirectX] Failed to create rasterizer state. Error: {}", HResultToString(hr));
        }

#if defined(FL_DEBUG) || defined(FL_FORCE_DX_DEBUG_INTERFACE)
        hr = m_RasterizerState->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof("Rasterizer State") - 1,
                                               "Rasterizer State");

        if (FAILED(hr)) {
            spdlog::error("[DirectX] Failed to set name for rasterizer state. Error: {}", HResultToString(hr));
        }
#endif

        CreateDepthStencilState();
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
        deviceContext->ClearDepthStencilView(m_Swapchain->GetDSV().Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);

        deviceContext->RSSetViewports(1, &viewport);
        deviceContext->OMSetRenderTargets(1, m_Swapchain->GetRTV().GetAddressOf(), m_Swapchain->GetDSV().Get());
        deviceContext->OMSetDepthStencilState(m_DepthStencilState.Get(), 0);
        deviceContext->RSSetState(m_RasterizerState.Get());
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

    void Renderer::BindVertexBuffers(const std::vector<ID3D11Buffer*>& buffers,
                                     const VertexType vertexType,
                                     const std::vector<UInt32>& offsets,
                                     const UInt32 startSlot) const {
        const UInt32 stride = ShaderCollection::GetLayoutByteSize(vertexType);
        m_Device->GetDeviceContext()->IASetVertexBuffers(startSlot, static_cast<UInt32>(buffers.size()), buffers.data(),
                                                         &stride, offsets.data());
    }

    void Renderer::BindIndexBuffer(ID3D11Buffer* buffer, const IndexType indexType) const {
        DXGI_FORMAT format = DXGI_FORMAT_UNKNOWN;
        switch (indexType) {
        case IndexType::UInt16:
            format = DXGI_FORMAT_R16_UINT;
            break;
        case IndexType::UInt32:
            format = DXGI_FORMAT_R32_UINT;
            break;
        }

        m_Device->GetDeviceContext()->IASetIndexBuffer(buffer, format, 0);
    }


    void Renderer::BindConstantBuffers(const std::vector<ID3D11Buffer*>& buffers,
                                       const PipelineBindPoint bindPoint,
                                       const UInt32 startSlot) const {
        const auto deviceContext = m_Device->GetDeviceContext();

        switch (bindPoint) {
        case PipelineBindPoint::VertexShader:
            deviceContext->VSSetConstantBuffers(startSlot, static_cast<UInt32>(buffers.size()), buffers.data());
            break;
        case PipelineBindPoint::PixelShader:
            deviceContext->PSSetConstantBuffers(startSlot, static_cast<UInt32>(buffers.size()), buffers.data());
            break;
        case PipelineBindPoint::GeometryShader:
        case PipelineBindPoint::HullShader:
        case PipelineBindPoint::DomainShader:
        case PipelineBindPoint::ComputeShader:
            spdlog::warn("[DirectX] Binding constant buffers to unsupported shader stage.");
            break;
        }
    }


    void Renderer::SetPrimitiveTopology(const PrimitiveTopology topology) const {
        m_Device->GetDeviceContext()->IASetPrimitiveTopology(
            static_cast<D3D11_PRIMITIVE_TOPOLOGY>(EnumAsInteger(topology))
        );
    }

    void Renderer::Draw(const UInt32 vertexCount, const UInt32 firstVertex) const {
        m_Device->GetDeviceContext()->Draw(vertexCount, firstVertex);
    }

    void Renderer::DrawIndexed(const UInt32 indexCount, const UInt32 firstIndex, const UInt32 baseVertex) const {
        m_Device->GetDeviceContext()->DrawIndexed(indexCount, firstIndex, baseVertex);
    }

    ShaderCollection Renderer::CreateShaderCollection(const VertexType vertexType,
                                                      const std::filesystem::path& vertexShaderPath,
                                                      const std::filesystem::path& pixelShaderPath,
                                                      const std::string_view name) const {
        ShaderCollectionDescriptor desc{};
        desc.VertexType = vertexType;
        desc.VertexShaderPath = Filesystem::GetShadersDirectory() / vertexShaderPath;
        desc.PixelShaderPath = Filesystem::GetShadersDirectory() / pixelShaderPath;
        desc.Name = name;

        ShaderCollection collection = ShaderCollection::CreateShaderCollection(desc, *m_Device);

        return collection;
    }

    std::unique_ptr<Buffer> Renderer::CreateBuffer(const void* data,
                                                   const UInt32 size,
                                                   const D3D11_USAGE usage,
                                                   const D3D11_BIND_FLAG bindFlag,
                                                   const std::string_view name,
                                                   const bool hasCpuAccess,
                                                   const D3D11_CPU_ACCESS_FLAG cpuAccess) const {
        return std::make_unique<Buffer>(m_Device, data, size, usage, bindFlag, name, hasCpuAccess, cpuAccess);
    }

    std::unique_ptr<Buffer> Renderer::CreateEmptyBuffer(const UInt32 size,
                                                        const D3D11_USAGE usage,
                                                        const D3D11_BIND_FLAG bindFlag,
                                                        const std::string_view name,
                                                        const bool hasCpuAccess,
                                                        const D3D11_CPU_ACCESS_FLAG cpuAccess) const {
        return std::make_unique<Buffer>(m_Device, size, usage, bindFlag, name, hasCpuAccess, cpuAccess);
    }


    std::shared_ptr<Sampler> Renderer::CreateSampler(const D3D11_FILTER filter,
                                                     const std::string_view name,
                                                     const D3D11_TEXTURE_ADDRESS_MODE addressModeU,
                                                     const D3D11_TEXTURE_ADDRESS_MODE addressModeV,
                                                     const D3D11_TEXTURE_ADDRESS_MODE addressModeW
    ) const {
        return std::make_shared<Sampler>(m_Device, filter, name, addressModeU, addressModeV, addressModeW);
    }

    std::unique_ptr<Texture> Renderer::CreateTexture(const std::filesystem::path& path,
                                                     const std::string_view name) const {
        return std::make_unique<Texture>(path, name, m_Device);
    }

    void Renderer::CreateDepthStencilState() {

        D3D11_DEPTH_STENCIL_DESC depthStencilDesc{};
        depthStencilDesc.DepthEnable = true;
        depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
        depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        HRESULT hr = m_Device->GetDevice()->CreateDepthStencilState(&depthStencilDesc, m_DepthStencilState.GetAddressOf());
        if (FAILED(hr)) {
            spdlog::error("[DirectX] Failed to create depth stencil state. Error: {}", HResultToString(hr));
        }

#if defined(FL_DEBUG) || defined(FL_FORCE_DX_DEBUG_INTERFACE)
        hr = m_DepthStencilState->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof("Depth Stencil State") - 1,
                                                 "Depth Stencil State");
        if (FAILED(hr)) {
            spdlog::error("[DirectX] Failed to set name for depth stencil state. Error: {}", HResultToString(hr));
        }
#endif
    }
}
