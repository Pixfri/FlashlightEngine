// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#ifndef FL_RENDERER_RENDERER_HPP
#define FL_RENDERER_RENDERER_HPP

#include <FlashlightEngine/Types.hpp>

#include <FlashlightEngine/Core/Window.hpp>

#include <FlashlightEngine/Renderer/Device.hpp>
#include <FlashlightEngine/Renderer/Swapchain.hpp>
#include <FlashlightEngine/Renderer/Shader.hpp>
#include <FlashlightEngine/Renderer/Buffer.hpp>
#include <FlashlightEngine/Renderer/Enums.hpp>
#include <FlashlightEngine/Renderer/Texture.hpp>
#include <FlashlightEngine/Renderer/Sampler.hpp>

namespace FlashlightEngine {
    class Renderer {
    public:
        explicit Renderer(const std::shared_ptr<Window>& window);
        ~Renderer() = default;

        Renderer(const Renderer&) = delete;
        Renderer(Renderer&&) noexcept = default;

        void OnResize(UInt32 width, UInt32 height) const;

        void BeginFrame() const;
        void EndFrame() const;

        inline void SetClearColor(Float32 r, Float32 g, Float32 b, Float32 a);
        void UseShaderCollection(const ShaderCollection& collection) const;
        void BindVertexBuffer(const Buffer& buffer, VertexType vertexType, UInt32 offset = 0) const;
        void SetPrimitiveTopology(PrimitiveTopology topology) const;
        void Draw(UInt32 vertexCount, UInt32 firstVertex = 0) const;

        [[nodiscard]] ShaderCollection CreateShaderCollection(VertexType vertexType,
                                                              const std::filesystem::path& vertexShaderPath,
                                                              const std::filesystem::path& pixelShaderPath,
                                                              std::string_view name) const;
        std::unique_ptr<Buffer> CreateBuffer(const void* data,
                                             UInt32 size,
                                             D3D11_USAGE usage,
                                             D3D11_BIND_FLAG bindFlag,
                                             std::string_view name = "Buffer",
                                             bool hasCpuAccess = false,
                                             D3D11_CPU_ACCESS_FLAG cpuAccess = D3D11_CPU_ACCESS_WRITE) const;
        [[nodiscard]]
        std::shared_ptr<Sampler> CreateSampler(D3D11_FILTER filter,
                                               std::string_view name = "Sampler",
                                               D3D11_TEXTURE_ADDRESS_MODE addressModeU = D3D11_TEXTURE_ADDRESS_WRAP,
                                               D3D11_TEXTURE_ADDRESS_MODE addressModeV = D3D11_TEXTURE_ADDRESS_WRAP,
                                               D3D11_TEXTURE_ADDRESS_MODE addressModeW = D3D11_TEXTURE_ADDRESS_WRAP) const;
        [[nodiscard]]
        std::unique_ptr<Texture> CreateTexture(const std::filesystem::path& path,
                                               std::string_view name = "Texture") const;

        Renderer& operator=(const Renderer&) = delete;
        Renderer& operator=(Renderer&&) noexcept = default;

    private:
        std::shared_ptr<Window> m_Window;

        std::shared_ptr<Device> m_Device;
        std::shared_ptr<Swapchain> m_Swapchain;

        Float32 m_ClearColor[4] = {0.1f, 0.1f, 0.1f, 0.1f};
    };
}

#include <FlashlightEngine/Renderer/Renderer.inl>

#endif // FL_RENDERER_RENDERER_HPP
