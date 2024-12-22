// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#ifndef FL_RENDERER_SHADER_HPP
#define FL_RENDERER_SHADER_HPP

#include <FlashlightEngine/Types.hpp>

#include <FlashlightEngine/Renderer/DxUtils.hpp>
#include <FlashlightEngine/Renderer/VertexTypes.hpp>
#include <FlashlightEngine/Renderer/Device.hpp>

#include <filesystem>
#include <string>
#include <unordered_map>

namespace FlashlightEngine {
    struct ShaderCollectionDescriptor {
        std::filesystem::path VertexShaderPath;
        std::filesystem::path PixelShaderPath;
        VertexType VertexType;
        std::string_view Name;
    };

    class ShaderCollection {
    public:
        static ShaderCollection CreateShaderCollection(const ShaderCollectionDescriptor& desc, const Device& device);
        static UInt32 GetLayoutByteSize(VertexType type);

        void ApplyToContext(const Device& device) const;
        void Destroy();

    private:
        [[nodiscard]] static ComPtr<ID3D11VertexShader> CreateVertexShader(const Device& device,
                                                                           const std::filesystem::path& filePath,
                                                                           ComPtr<ID3DBlob>& vertexShaderBlob);
        [[nodiscard]] static ComPtr<ID3D11PixelShader> CreatePixelShader(const Device& device,
                                                                          const std::filesystem::path& filePath);

        static bool CreateInputLayout(const Device& device,
                                      VertexType layoutInfo,
                                      const ComPtr<ID3DBlob>& vertexShaderBlob,
                                      ComPtr<ID3D11InputLayout>& inputLayout);

        static bool CompileShader(const std::filesystem::path& filePath,
                                  const std::string& entryPoint,
                                  const std::string& profile,
                                  ComPtr<ID3DBlob>& shaderBlob);

        ComPtr<ID3D11VertexShader> m_VertexShader{nullptr};
        ComPtr<ID3D11PixelShader> m_PixelShader{nullptr};
        ComPtr<ID3D11InputLayout> m_InputLayout{nullptr};
        D3D11_PRIMITIVE_TOPOLOGY m_PrimitiveTopology{};
        UInt32 m_VertexSize{0};
        static std::unordered_map<VertexType, std::vector<D3D11_INPUT_ELEMENT_DESC>> m_LayoutMap;
    };
}

#include <FlashlightEngine/Renderer/Shader.inl>

#endif // FL_RENDERER_SHADER_HPP
