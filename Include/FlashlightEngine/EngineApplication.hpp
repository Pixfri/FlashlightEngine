// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#ifndef FL_ENGINEAPPLICATION_HPP
#define FL_ENGINEAPPLICATION_HPP

#include <FlashlightEngine/Types.hpp>

#include <FlashlightEngine/Application.hpp>

#include <FlashlightEngine/Renderer/Shader.hpp>

#include <DirectXMath.h>

struct PerFrameConstantBuffer {
    DirectX::XMFLOAT4X4 ViewProjectionMatrix;
};

struct PerObjectConstantBuffer {
    DirectX::XMFLOAT4X4 ModelMatrix;
};

namespace FlashlightEngine {
    class EngineApplication final : public Application {
    public:
        EngineApplication(UInt32 width, UInt32 height);
        ~EngineApplication() override;

        void OnEvent(Event& event) override;
        void OnUpdate() override;
        void OnRender() override;

    private:
        ShaderCollection m_MainShaderCollection;
        std::unique_ptr<Buffer> m_CubeVertexBuffer;
        std::unique_ptr<Buffer> m_CubeIndexBuffer;

        std::shared_ptr<Sampler> m_LinearSampler;
        std::unique_ptr<Texture> m_FrogTexture;
        std::unique_ptr<Texture> m_FallbackTexture;

        std::unique_ptr<Buffer> m_PerFrameConstantBuffer = nullptr;
        std::unique_ptr<Buffer> m_PerObjectConstantBuffer = nullptr;

        PerFrameConstantBuffer m_PerFrameConstantBufferData{};
        PerObjectConstantBuffer m_PerObjectConstantBufferData{};

        bool m_UseFrogTexture = true;

        void OnKeyPressed(const KeyDownEvent& event);
    };
}

#include <FlashlightEngine/EngineApplication.inl>

#endif // FL_ENGINEAPPLICATION_HPP
