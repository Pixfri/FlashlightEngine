// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#include <FlashlightEngine/Renderer/Renderer.hpp>

#include <FlashlightEngine/Renderer/Pipeline.hpp>

#include <FlashlightEngine/Core/Filesystem.hpp>


namespace FlashlightEngine {
    Renderer::Renderer(const std::shared_ptr<Window>& window)
        : m_Window(window) {
        m_Device = std::make_shared<Device>();
        m_Swapchain = std::make_shared<Swapchain>(m_Window, m_Device);

        // Set the default primitive topology to be a triangle list.
        SetPrimitiveTopology(PrimitiveTopology::TriangleList);

        CreateDepthStencilState();
        CreateRasterizerState();
    }

    void Renderer::OnResize(const UInt32 width, const UInt32 height) const {
        m_Device->GetDeviceContext()->Flush();

        m_Swapchain->OnResize(width, height);
    }

    void Renderer::BeginFrame() const {
        const ComPtr<ID3D11DeviceContext> deviceContext = m_Device->GetDeviceContext();

        deviceContext->ClearRenderTargetView(m_Swapchain->GetRTV().Get(), m_ClearColor);
        deviceContext->ClearDepthStencilView(m_Swapchain->GetDSV().Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);

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

    void Renderer::SetPrimitiveTopology(const PrimitiveTopology topology) const {
        m_Device->GetDeviceContext()->IASetPrimitiveTopology(
            static_cast<D3D11_PRIMITIVE_TOPOLOGY>(EnumAsInteger(topology))
        );
    }

    void Renderer::SetPipeline(const Pipeline* pipeline) {
        m_ActivePipeline = pipeline;
        m_Device->GetDeviceContext()->IASetInputLayout(pipeline->m_InputLayout.Get());
        m_Device->GetDeviceContext()->IASetPrimitiveTopology(pipeline->m_Topology);
        m_Device->GetDeviceContext()->VSSetShader(pipeline->m_VertexShader.Get(), nullptr, 0);
        m_Device->GetDeviceContext()->PSSetShader(pipeline->m_PixelShader.Get(), nullptr, 0);

        const auto deviceContext = m_Device->GetDeviceContext();

        for (auto [descriptor, resource] : pipeline->m_Resources) {
            switch (descriptor.Type) {
            case ResourceType::Sampler:
                BindSamplerToPipeline(descriptor.Stage, descriptor.SlotIndex,
                                      reinterpret_cast<ID3D11SamplerState**>(&resource));
                break;
            case ResourceType::Texture:
                BindTextureToPipeline(descriptor.Stage, descriptor.SlotIndex,
                                      reinterpret_cast<ID3D11ShaderResourceView**>(&resource));
                break;
            case ResourceType::Buffer:
                BindConstantBufferToPipeline(descriptor.Stage, descriptor.SlotIndex,
                                             reinterpret_cast<ID3D11Buffer**>(&resource));
                break;
            }
        }

        m_Device->GetDeviceContext()->RSSetViewports(1, &pipeline->m_Viewport);
    }

    void Renderer::SetVertexBuffer(const Buffer& buffer, const UInt32 vertexOffset) {
        if (!m_ActivePipeline) {
            spdlog::warn("[DirectX] Trying to set vertex buffer with no pipeline bound.");
            return;
        }

        D3D11_BUFFER_DESC desc{};
        buffer.GetBuffer()->GetDesc(&desc);

        if (!(desc.BindFlags & D3D11_BIND_VERTEX_BUFFER)) {
            spdlog::warn("[DirectX] Trying to bind a vertex buffer that is not marked as such.");
            return;
        }

        m_Device->GetDeviceContext()->IASetVertexBuffers(
            0,
            1,
            buffer.GetBuffer().GetAddressOf(),
            &m_ActivePipeline->m_VertexSize,
            &vertexOffset
        );

        m_DrawVertices = desc.ByteWidth / m_ActivePipeline->m_VertexSize;
    }

    void Renderer::SetIndexBuffer(const Buffer& buffer, const UInt32 indexOffset, const IndexType indexType) {
        D3D11_BUFFER_DESC desc{};
        buffer.GetBuffer()->GetDesc(&desc);

        if (!(desc.BindFlags & D3D11_BIND_INDEX_BUFFER)) {
            spdlog::warn("[DirectX] Trying to bind an index buffer that is not marked as such.");
            return;
        }

        m_Device->GetDeviceContext()->IASetIndexBuffer(
            buffer.GetBuffer().Get(),
            indexType == IndexType::UInt32 ? DXGI_FORMAT_R32_UINT : DXGI_FORMAT_R16_UINT,
            indexOffset);

        const UInt64 indexSize = indexType == IndexType::UInt32 ? sizeof(UInt32) : sizeof(UInt16);
        m_DrawIndices = desc.ByteWidth / indexSize;
    }

    void Renderer::UpdateSubresource(const Buffer& buffer, const void* data) const {
        m_Device->GetDeviceContext()->UpdateSubresource(
            buffer.GetBuffer().Get(),
            0,
            nullptr,
            data,
            0,
            0);
    }

    void Renderer::Draw() const {
        m_Device->GetDeviceContext()->Draw(m_DrawVertices, 0);
    }

    void Renderer::DrawIndexed() const {
        m_Device->GetDeviceContext()->DrawIndexed(m_DrawIndices, 0, 0);
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

    std::unique_ptr<PipelineBuilder> Renderer::CreatePipelineBuilder() const {
        return std::make_unique<PipelineBuilder>(m_Device);
    }

    void Renderer::CreateDepthStencilState() {
        D3D11_DEPTH_STENCIL_DESC depthStencilDesc{};
        depthStencilDesc.DepthEnable = true;
        depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
        depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        HRESULT hr = m_Device->GetDevice()->CreateDepthStencilState(&depthStencilDesc,
                                                                    m_DepthStencilState.GetAddressOf());
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

    void Renderer::CreateRasterizerState() {
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
    }

    void Renderer::BindSamplerToPipeline(const PipelineBindPoint bindPoint, const UInt32 slotIndex,
                                         ID3D11SamplerState** sampler) const {
        const auto deviceContext = m_Device->GetDeviceContext();

        switch (bindPoint) {
        case PipelineBindPoint::VertexShader:
            deviceContext->VSSetSamplers(slotIndex, 1, sampler);
            break;
        case PipelineBindPoint::PixelShader:
            deviceContext->PSSetSamplers(slotIndex, 1, sampler);
            break;
        case PipelineBindPoint::ComputeShader:
        case PipelineBindPoint::DomainShader:
        case PipelineBindPoint::HullShader:
        case PipelineBindPoint::GeometryShader:
            spdlog::warn("[DirectX] Unsupported shader stage when binding sampler.");
            break;
        }
    }

    void Renderer::BindTextureToPipeline(const PipelineBindPoint bindPoint, const UInt32 slotIndex,
                                         ID3D11ShaderResourceView** texture) const {
        const auto deviceContext = m_Device->GetDeviceContext();

        switch (bindPoint) {
        case PipelineBindPoint::VertexShader:
            deviceContext->VSSetShaderResources(slotIndex, 1, texture);
            break;
        case PipelineBindPoint::PixelShader:
            deviceContext->PSSetShaderResources(slotIndex, 1, texture);
            break;
        case PipelineBindPoint::ComputeShader:
        case PipelineBindPoint::DomainShader:
        case PipelineBindPoint::HullShader:
        case PipelineBindPoint::GeometryShader:
            spdlog::warn("[DirectX] Unsupported shader stage when binding texture.");
            break;
        }
    }

    void Renderer::BindConstantBufferToPipeline(const PipelineBindPoint bindPoint, const UInt32 slotIndex,
                                                ID3D11Buffer** buffer) const {
        const auto deviceContext = m_Device->GetDeviceContext();

        switch (bindPoint) {
        case PipelineBindPoint::VertexShader:
            deviceContext->VSSetConstantBuffers(slotIndex, 1, buffer);
            break;
        case PipelineBindPoint::PixelShader:
            deviceContext->PSSetConstantBuffers(slotIndex, 1, buffer);
            break;
        case PipelineBindPoint::ComputeShader:
        case PipelineBindPoint::DomainShader:
        case PipelineBindPoint::HullShader:
        case PipelineBindPoint::GeometryShader:
            spdlog::warn("[DirectX] Unsupported shader stage when binding constant buffer.");
            break;
        }
    }
}
