// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#include <FlashlightEngine/Renderer/Pipeline.hpp>

#include <d3dcompiler.h>

namespace FlashlightEngine {
    void Pipeline::BindTexture(const UInt32 slotIndex, const Texture& texture, const PipelineBindPoint bindPoint) {
        ResourceDescriptor desc{};
        desc.SlotIndex = slotIndex;
        desc.Type = ResourceType::Texture;
        desc.Stage = bindPoint;
        m_Resources[desc] = texture.GetTextureSRV().Get();
    }

    void Pipeline::BindSampler(const UInt32 slotIndex, const Sampler& sampler, const PipelineBindPoint bindPoint) {
        ResourceDescriptor desc{};
        desc.SlotIndex = slotIndex;
        desc.Type = ResourceType::Sampler;
        desc.Stage = bindPoint;
        m_Resources[desc] = sampler.GetSampler().Get();
    }

    void Pipeline::BindConstantBuffer(const UInt32 slotIndex, const Buffer& buffer, const PipelineBindPoint bindPoint) {
        ResourceDescriptor desc{};
        desc.SlotIndex = slotIndex;
        desc.Type = ResourceType::Buffer;
        desc.Stage = bindPoint;
        m_Resources[desc] = buffer.GetBuffer().Get();
    }

    void Pipeline::SetViewport(const Float32 left, const Float32 top, const Float32 width, const Float32 height) {
        m_Viewport.TopLeftX = left;
        m_Viewport.TopLeftY = top;
        m_Viewport.Width = width;
        m_Viewport.Height = height;
        m_Viewport.MinDepth = 0.0f;
        m_Viewport.MaxDepth = 1.0f;
    }

    PipelineBuilder::PipelineBuilder(const std::shared_ptr<Device>& device)
        : m_Device(device) {
        m_LayoutMap[VertexType::PositionColor] = {
            {
                {
                    "POSITION",
                    0,
                    DXGI_FORMAT_R32G32B32_FLOAT,
                    0,
                    offsetof(VertexPositionColor, Position),
                    D3D11_INPUT_PER_VERTEX_DATA,
                    0
                },
                {
                    "COLOR",
                    0,
                    DXGI_FORMAT_R32G32B32_FLOAT,
                    0,
                    offsetof(VertexPositionColor, Color),
                    D3D11_INPUT_PER_VERTEX_DATA,
                    0
                }
            }
        };

        m_LayoutMap[VertexType::PositionColorUv] = {
            {
                {
                    "POSITION",
                    0,
                    DXGI_FORMAT_R32G32B32_FLOAT,
                    0,
                    offsetof(VertexPositionColorUv, Position),
                    D3D11_INPUT_PER_VERTEX_DATA,
                    0
                },
                {
                    "COLOR",
                    0,
                    DXGI_FORMAT_R32G32B32_FLOAT,
                    0,
                    offsetof(VertexPositionColorUv, Color),
                    D3D11_INPUT_PER_VERTEX_DATA,
                    0
                },
                {
                    "TEXCOORD",
                    0,
                    DXGI_FORMAT_R32G32_FLOAT,
                    0,
                    offsetof(VertexPositionColorUv, Uv),
                    D3D11_INPUT_PER_VERTEX_DATA,
                    0
                }
            }
        };
    }

    bool PipelineBuilder::CreatePipeline(const PipelineDescriptor& settings, std::unique_ptr<Pipeline>& pipeline) {
        ComPtr<ID3DBlob> vertexShaderBlob;
        pipeline = std::make_unique<Pipeline>();
        pipeline->m_VertexShader = CreateVertexShader(settings.VertexShaderPath, vertexShaderBlob);
        pipeline->m_PixelShader = CreatePixelShader(settings.PixelShaderPath);
        if (!CreateInputLayout(settings.VertexType, vertexShaderBlob, pipeline->m_InputLayout)) {
            return false;
        }
        pipeline->m_Topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
        pipeline->m_VertexSize = static_cast<UInt32>(GetLayoutByteSize(settings.VertexType));

        spdlog::info("[DirectX] Pipeline {} created.", settings.PipelineName);

#if defined(FL_DEBUG) || defined(FL_FORCE_DX_DEBUG_INTERFACE)
        HRESULT hr = pipeline->m_VertexShader->SetPrivateData(
            WKPDID_D3DDebugObjectName,
            static_cast<UInt32>(std::string("VS_" + settings.PipelineName).size()),
            ("VS_" + settings.PipelineName).c_str());

        if (FAILED(hr)) {
            spdlog::error("[DirectX] Failed to set vertex shader debug name. Error: {}", HResultToString(hr));
            return false;
        }

        hr = pipeline->m_PixelShader->SetPrivateData(
            WKPDID_D3DDebugObjectName,
            static_cast<UInt32>(std::string("PS_" + settings.PipelineName).size()),
            ("PS_" + settings.PipelineName).c_str());

        if (FAILED(hr)) {
            spdlog::error("[DirectX] Failed to set pixel shader debug name. Error: {}", HResultToString(hr));
            return false;
        }

        hr = pipeline->m_InputLayout->SetPrivateData(
            WKPDID_D3DDebugObjectName,
            static_cast<UInt32>(std::string("IL_" + settings.PipelineName).size()),
            ("IL_" + settings.PipelineName).c_str());

        if (FAILED(hr)) {
            spdlog::error("[DirectX] Failed to set input layout debug name. Error: {}", HResultToString(hr));
            return false;
        }
#endif

        return true;
    }

    UInt64 PipelineBuilder::GetLayoutByteSize(const VertexType vertexType) {
        switch (vertexType) {
        case VertexType::PositionColor:
            return sizeof(VertexPositionColor);
        case VertexType::PositionColorUv:
            return sizeof(VertexPositionColorUv);
        default:
            return 0;
        }
    }

    ComPtr<ID3D11VertexShader> PipelineBuilder::CreateVertexShader(const std::filesystem::path& path,
                                                                   ComPtr<ID3DBlob>& vertexShaderBlob) {
        if (!CompileShader(path, "Main", "vs_5_0", vertexShaderBlob)) {
            spdlog::error("[DirectX] Failed to compile vertex shader: {}", path.filename().string());
            return nullptr;
        }

        ComPtr<ID3D11VertexShader> vertexShader;
        const HRESULT hr = m_Device->GetDevice()->CreateVertexShader(vertexShaderBlob->GetBufferPointer(),
                                                                     vertexShaderBlob->GetBufferSize(),
                                                                     nullptr,
                                                                     vertexShader.GetAddressOf());

        if (FAILED(hr)) {
            spdlog::error("[DirectX] Failed to create vertex shader: {}. Error: {}", path.filename().string(),
                          HResultToString(hr));
            return nullptr;
        }

        return vertexShader;
    }

    ComPtr<ID3D11PixelShader> PipelineBuilder::CreatePixelShader(const std::filesystem::path& path) {
        ComPtr<ID3DBlob> pixelShaderBlob;
        if (!CompileShader(path, "Main", "ps_5_0", pixelShaderBlob)) {
            spdlog::error("[DirectX] Failed to compile pixel shader: {}", path.filename().string());
            return nullptr;
        }

        ComPtr<ID3D11PixelShader> pixelShader;
        const HRESULT hr = m_Device->GetDevice()->CreatePixelShader(pixelShaderBlob->GetBufferPointer(),
                                                                    pixelShaderBlob->GetBufferSize(),
                                                                    nullptr,
                                                                    pixelShader.GetAddressOf());

        if (FAILED(hr)) {
            spdlog::error("[DirectX] Failed to create vertex shader: {}. Error: {}", path.filename().string(),
                          HResultToString(hr));
            return nullptr;
        }

        return pixelShader;
    }

    bool PipelineBuilder::CreateInputLayout(const VertexType layoutInfo,
                                            const ComPtr<ID3DBlob>& vertexBlob,
                                            ComPtr<ID3D11InputLayout>& inputLayout) {
        const std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc = m_LayoutMap[layoutInfo];
        const HRESULT hr = m_Device->GetDevice()->CreateInputLayout(inputLayoutDesc.data(),
                                                                    static_cast<UInt32>(inputLayoutDesc.size()),
                                                                    vertexBlob->GetBufferPointer(),
                                                                    vertexBlob->GetBufferSize(),
                                                                    inputLayout.GetAddressOf());
        if (FAILED(hr)) {
            spdlog::error("[DirectX] Failed to create input layout. Error: {}", HResultToString(hr));
            return false;
        }

        return true;
    }

    bool PipelineBuilder::CompileShader(const std::filesystem::path& path,
                                        const std::string& entryPoint,
                                        const std::string& profile,
                                        ComPtr<ID3DBlob>& shaderBlob) {
        UInt32 compileFlags = D3DCOMPILE_ENABLE_STRICTNESS;

#if defined(FL_DEBUG) || defined(FL_FORCE_DX_DEBUG_INTERFACE)
        compileFlags |= D3DCOMPILE_DEBUG;
#endif

        ComPtr<ID3DBlob> tempShaderBlob = nullptr;
        ComPtr<ID3DBlob> errorBlob = nullptr;

        const HRESULT hr = D3DCompileFromFile((Filesystem::GetShadersDirectory() / path).c_str(),
                                              nullptr,
                                              D3D_COMPILE_STANDARD_FILE_INCLUDE,
                                              entryPoint.c_str(),
                                              profile.c_str(),
                                              compileFlags,
                                              0,
                                              &tempShaderBlob,
                                              &errorBlob);
        if (FAILED(hr)) {
            spdlog::error("[DirectX] Failed to compile shader: {}. Error: {}", path.filename().string(),
                          HResultToString(hr));

            if (errorBlob != nullptr) {
                spdlog::error("[DirectX] Additional compiler errors: {}",
                              static_cast<const char*>(errorBlob->GetBufferPointer()));
            }

            return false;
        }

        shaderBlob = std::move(tempShaderBlob);

        return true;
    }
}
