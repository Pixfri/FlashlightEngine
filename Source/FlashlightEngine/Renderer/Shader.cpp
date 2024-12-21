// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#include <FlashlightEngine/Renderer/Shader.hpp>

#include <FlashlightEngine/Utils/FileUtils.hpp>

#include <d3dcompiler.h>

#include <spdlog/spdlog.h>

namespace FlashlightEngine {
    std::unordered_map<VertexType, std::vector<D3D11_INPUT_ELEMENT_DESC>> ShaderCollection::m_LayoutMap = {
        {
            VertexType::PositionColor,
            {
                {
                    "POSITION",
                    0, DXGI_FORMAT_R32G32B32_FLOAT,
                    0,
                    offsetof(VertexPositionColor, Pos),
                    D3D11_INPUT_PER_VERTEX_DATA,
                    0
                },
                {
                    "COLOR",
                    0,
                    DXGI_FORMAT_R32G32B32_FLOAT,
                    0,
                    offsetof(VertexPositionColor, Col),
                    D3D11_INPUT_PER_VERTEX_DATA,
                    0
                }
            }
        }
    };

    ShaderCollection ShaderCollection::CreateShaderCollection(const ShaderCollectionDescriptor& desc,
                                                              const Device& device) {
        ShaderCollection collection;

        ComPtr<ID3DBlob> vertexShaderBlob;
        collection.m_VertexShader = CreateVertexShader(device, desc.VertexShaderPath, vertexShaderBlob);
        collection.m_PixelShader = CreatePixelShader(device, desc.PixelShaderPath);
        if (!CreateInputLayout(device, desc.VertexType, vertexShaderBlob, collection.m_InputLayout)) {
            spdlog::error("[DirectX] Failed to create input layout for shader collection.");
            return {};
        }

        return collection;
    }

    UInt32 ShaderCollection::GetLayoutByteSize(const VertexType type) {
        switch (type) {
        case VertexType::PositionColor:
            return sizeof(VertexPositionColor);
        default:
            return 0;
        }
    }

    ComPtr<ID3D11VertexShader> ShaderCollection::CreateVertexShader(const Device& device,
                                                                    const std::filesystem::path& filePath,
                                                                    ComPtr<ID3DBlob>& vertexShaderBlob) {
        if (!CompileShader(filePath, "Main", "vs_5_0", vertexShaderBlob)) {
            spdlog::error("[DirectX] Failed to compile shader: {}", filePath.string());
            return nullptr;
        }

        ComPtr<ID3D11VertexShader> vertexShader;
        const HRESULT hr = device.GetDevice()->CreateVertexShader(vertexShaderBlob->GetBufferPointer(),
                                                                  vertexShaderBlob->GetBufferSize(),
                                                                  nullptr,
                                                                  &vertexShader);
        if (FAILED(hr)) {
            spdlog::error("[DirectX] Failed to create vertex shader. Error: {}", HResultToString(hr));
            return nullptr;
        }

        return vertexShader;
    }

    ComPtr<ID3D11PixelShader> ShaderCollection::CreatePixelShader(const Device& device,
                                                                  const std::filesystem::path& filePath) {
        ComPtr<ID3DBlob> pixelShaderBlob = nullptr;
        if (!CompileShader(filePath, "Main", "ps_5_0", pixelShaderBlob)) {
            spdlog::error("[DirectX] Failed to compile shader: {}", filePath.string());
            return nullptr;
        }

        ComPtr<ID3D11PixelShader> pixelShader;
        const HRESULT hr = device.GetDevice()->CreatePixelShader(pixelShaderBlob->GetBufferPointer(),
                                                                 pixelShaderBlob->GetBufferSize(),
                                                                 nullptr,
                                                                 &pixelShader);

        if (FAILED(hr)) {
            spdlog::error("[DirectX] Failed to create pixel shader. Error: {}", HResultToString(hr));
            return nullptr;
        }

        return pixelShader;
    }

    bool ShaderCollection::CreateInputLayout(const Device& device,
                                             const VertexType layoutInfo,
                                             const ComPtr<ID3DBlob>& vertexShaderBlob,
                                             ComPtr<ID3D11InputLayout>& inputLayout) {
        const std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc = m_LayoutMap[layoutInfo];

        const HRESULT hr = device.GetDevice()->CreateInputLayout(inputLayoutDesc.data(),
                                                                 static_cast<UInt32>(inputLayoutDesc.size()),
                                                                 vertexShaderBlob->GetBufferPointer(),
                                                                 vertexShaderBlob->GetBufferSize(),
                                                                 &inputLayout);

        if (FAILED(hr)) {
            spdlog::error("[DirectX] Failed to create input layout. Error: {}", HResultToString(hr));
            return false;
        }

        return true;
    }

    bool ShaderCollection::CompileShader(const std::filesystem::path& filePath,
                                         const std::string& entryPoint,
                                         const std::string& profile,
                                         ComPtr<ID3DBlob>& shaderBlob) {
        UInt32 compileFlags = D3DCOMPILE_ENABLE_STRICTNESS;

#if defined(FL_DEBUG) || defined(FL_FORCE_DX_DEBUG_INTERFACE)
        compileFlags |= D3DCOMPILE_DEBUG;
#endif

        ComPtr<ID3DBlob> tempShaderBlob = nullptr;
        ComPtr<ID3DBlob> errorBlob = nullptr;

        const HRESULT hr = D3DCompileFromFile(filePath.c_str(),
                                              nullptr,
                                              D3D_COMPILE_STANDARD_FILE_INCLUDE,
                                              entryPoint.c_str(),
                                              profile.c_str(),
                                              compileFlags,
                                              0,
                                              &tempShaderBlob,
                                              &errorBlob);
        if (FAILED(hr)) {
            spdlog::error("[DirectX] Failed to compile shader: {}. Error: {}", filePath.string(), HResultToString(hr));

            if (errorBlob) {
                spdlog::error("[DirectX] Shader compilation error: {}",
                              static_cast<const char*>(errorBlob->GetBufferPointer()));
            }

            return false;
        }

        shaderBlob = tempShaderBlob;

        return true;
    }

    void ShaderCollection::ApplyToContext(const Device& device) const {
        device.GetDeviceContext()->IASetInputLayout(m_InputLayout.Get());
        device.GetDeviceContext()->VSSetShader(m_VertexShader.Get(), nullptr, 0);
        device.GetDeviceContext()->PSSetShader(m_PixelShader.Get(), nullptr, 0);
    }

    void ShaderCollection::Destroy() {
        m_InputLayout.Reset();
        m_VertexShader.Reset();
        m_PixelShader.Reset();
    }
}
