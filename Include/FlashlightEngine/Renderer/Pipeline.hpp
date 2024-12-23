// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#ifndef FL_RENDERER_PIPELINE_HPP
#define FL_RENDERER_PIPELINE_HPP

#include <FlashlightEngine/Types.hpp>

#include <FlashlightEngine/Renderer/Buffer.hpp>
#include <FlashlightEngine/Renderer/Device.hpp>
#include <FlashlightEngine/Renderer/Enums.hpp>
#include <FlashlightEngine/Renderer/ResourceDescriptor.hpp>
#include <FlashlightEngine/Renderer/Sampler.hpp>
#include <FlashlightEngine/Renderer/Texture.hpp>
#include <FlashlightEngine/Renderer/VertexTypes.hpp>

#include <FlashlightEngine/Core/Filesystem.hpp>


namespace FlashlightEngine {
    struct PipelineDescriptor {
        std::filesystem::path VertexShaderPath;
        std::filesystem::path PixelShaderPath;
        VertexType VertexType;
        std::string PipelineName;
    };

    class Pipeline {
    public:
        friend class PipelineBuilder;
        friend class Renderer;

        void BindTexture(UInt32 slotIndex, const Texture& texture, PipelineBindPoint bindPoint);
        void BindSampler(UInt32 slotIndex, const Sampler& sampler, PipelineBindPoint bindPoint);
        void BindConstantBuffer(UInt32 slotIndex, const Buffer& buffer, PipelineBindPoint bindPoint);
        void SetViewport(Float32 left, Float32 top, Float32 width, Float32 height);

    private:
        ComPtr<ID3D11VertexShader> m_VertexShader{nullptr};
        ComPtr<ID3D11PixelShader> m_PixelShader{nullptr};
        ComPtr<ID3D11InputLayout> m_InputLayout{nullptr};
        std::unordered_map<ResourceDescriptor, ID3D11DeviceChild*> m_Resources;
        D3D11_PRIMITIVE_TOPOLOGY m_Topology{};
        UInt32 m_VertexSize{0};
        D3D11_VIEWPORT m_Viewport{};
    };

    class PipelineBuilder {
    public:
        explicit PipelineBuilder(const std::shared_ptr<Device>& device);
        ~PipelineBuilder() = default;

        PipelineBuilder(const PipelineBuilder&) = delete;
        PipelineBuilder(PipelineBuilder&&) = delete;

        bool CreatePipeline(const PipelineDescriptor& settings,
                            std::unique_ptr<Pipeline>& pipeline);

        PipelineBuilder& operator=(const PipelineBuilder&) = delete;
        PipelineBuilder& operator=(PipelineBuilder&&) = delete;

    private:
        static UInt64 GetLayoutByteSize(VertexType vertexType);

        ComPtr<ID3D11VertexShader> CreateVertexShader(const std::filesystem::path& path,
                                                      ComPtr<ID3DBlob>& vertexShaderBlob);

        ComPtr<ID3D11PixelShader> CreatePixelShader(const std::filesystem::path& path);

        bool CreateInputLayout(VertexType layoutInfo, const ComPtr<ID3DBlob>& vertexBlob,
                               ComPtr<ID3D11InputLayout>& inputLayout);

        bool CompileShader(const std::filesystem::path& path,
                           const std::string& entryPoint,
                           const std::string& profile,
                           ComPtr<ID3DBlob>& shaderBlob);

        std::shared_ptr<Device> m_Device{nullptr};
        std::unordered_map<VertexType, std::vector<D3D11_INPUT_ELEMENT_DESC>> m_LayoutMap;
    };
}

#include <FlashlightEngine/Renderer/Pipeline.inl>

#endif // FL_RENDERER_PIPELINE_HPP
