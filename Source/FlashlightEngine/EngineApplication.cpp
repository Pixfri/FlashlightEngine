// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#include <FlashlightEngine/EngineApplication.hpp>

#include <FlashlightEngine/Core/Input.hpp>
#include <FlashlightEngine/Core/KeyCodes.hpp>
#include <FlashlightEngine/Core/Filesystem.hpp>
#include <FlashlightEngine/Core/Time.hpp>

namespace FlashlightEngine {
    EngineApplication::EngineApplication(const UInt32 width, const UInt32 height)
        : Application(width, height, "Flashlight Engine <Direct3D 11>") {
        m_MainShaderCollection = m_Renderer->CreateShaderCollection(
            VertexType::PositionColorUv,
            "Main.vs.hlsl",
            "Main.ps.hlsl",
            "MainShaderCollection"
        );

        constexpr VertexPositionColorUv vertices[] = {
            {Position{0.0f, 0.5f, 0.0f}, Color{0.25f, 0.39f, 0.19f}, Uv{0.5f, 0.0f}},
            {Position{0.5f, -0.5f, 0.0f}, Color{0.44f, 0.75f, 0.35f}, Uv{1.0f, 1.0f}},
            {Position{-0.5f, -0.5f, 0.0f}, Color{0.38f, 0.55f, 0.20f}, Uv{0.0f, 1.0f}},
        };

        // RGB triangle
        //constexpr VertexPositionColorUv vertices[] = {
        //    {Position{0.0f, 0.5f, 0.0f}, Color{1.0f, 0.0f, 0.0f}, Uv{0.5f, 0.0f}},
        //    {Position{0.5f, -0.5f, 0.0f}, Color{0.0f, 1.0f, 0.0f}, Uv{1.0f, 1.0f}},
        //    {Position{-0.5f, -0.5f, 0.0f}, Color{0.0f, 0.0f, 1.0f}, Uv{0.0f, 1.0f}},
        //};

        m_TriangleVertexBuffer = m_Renderer->CreateBuffer(vertices, sizeof(vertices), D3D11_USAGE_IMMUTABLE,
                                                          D3D11_BIND_VERTEX_BUFFER, "Triangle Vertex Buffer");

        m_PerFrameConstantBuffer = m_Renderer->CreateEmptyBuffer(sizeof(PerFrameConstantBuffer),
                                                                 D3D11_USAGE_DYNAMIC,
                                                                 D3D11_BIND_CONSTANT_BUFFER,
                                                                 "Per Frame Constant Buffer",
                                                                 true);

        m_PerObjectConstantBuffer = m_Renderer->CreateEmptyBuffer(sizeof(PerObjectConstantBuffer),
                                                                  D3D11_USAGE_DYNAMIC,
                                                                  D3D11_BIND_CONSTANT_BUFFER,
                                                                  "Per Object Constant Buffer",
                                                                  true);

        m_LinearSampler = m_Renderer->CreateSampler(D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT, "Linear Sampler");

        m_FallbackTexture = m_Renderer->CreateTexture("Default.png", "Fallback texture");
        m_FrogTexture = m_Renderer->CreateTexture("T_Froge.dds", "Frog texture");

        m_Renderer->SetClearColor(0.0f, 0.2f, 0.4f, 1.0f);
    }

    EngineApplication::~EngineApplication() {
        m_MainShaderCollection.Destroy();
    }

    void EngineApplication::OnEvent(Event& event) {
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<KeyDownEvent>(FL_BIND_EVENT(EngineApplication::OnKeyPressed));
    }

    void EngineApplication::OnUpdate() {
        static Float32 yRotation = 0.0f;
        static Float32 scale = 1.0f;
        static DirectX::XMFLOAT3 cameraPosition = {0.0f, 0.0f, -1.0f};

        yRotation += Time::GetDeltaTime();

        // Camera
        const DirectX::XMVECTOR camPos = DirectX::XMLoadFloat3(&cameraPosition);

        const DirectX::XMMATRIX view = DirectX::XMMatrixLookAtRH(camPos, DirectX::g_XMZero, {0, 1, 0, 1});
        const DirectX::XMMATRIX proj = DirectX::XMMatrixPerspectiveFovRH(90.0f * 0.0174533f,
                                                                         static_cast<Float32>(GetWindow().GetWidth()) /
                                                                         static_cast<Float32>(GetWindow().GetHeight()),
                                                                         0.1f,
                                                                         100.0f);

        //combine the view & proj matrix
        const DirectX::XMMATRIX viewProj = DirectX::XMMatrixMultiply(view, proj);
        DirectX::XMStoreFloat4x4(&m_PerFrameConstantBufferData.ViewProjectionMatrix, viewProj);

        // Object transformation
        const DirectX::XMMATRIX translation = DirectX::XMMatrixTranslation(0, 0, 0);
        const DirectX::XMMATRIX scaling = DirectX::XMMatrixScaling(scale, scale, scale);
        const DirectX::XMMATRIX rotation = DirectX::XMMatrixRotationRollPitchYaw(0, yRotation, 0);

        //Now we create our model matrix
        const DirectX::XMMATRIX model = DirectX::XMMatrixMultiply(translation,
                                                                  DirectX::XMMatrixMultiply(scaling, rotation));
        DirectX::XMStoreFloat4x4(&m_PerObjectConstantBufferData.ModelMatrix, model);

        // Update the buffer's contents
        D3D11_MAPPED_SUBRESOURCE mappedResource;
        m_PerFrameConstantBuffer->Map(&mappedResource, D3D11_MAP_WRITE_DISCARD);
        std::memcpy(mappedResource.pData, &m_PerFrameConstantBufferData, sizeof(PerFrameConstantBuffer));
        m_PerFrameConstantBuffer->Unmap();

        m_PerObjectConstantBuffer->Map(&mappedResource, D3D11_MAP_WRITE_DISCARD);
        std::memcpy(mappedResource.pData, &m_PerObjectConstantBufferData, sizeof(PerObjectConstantBuffer));
        m_PerObjectConstantBuffer->Unmap();
    }

    void EngineApplication::OnRender() {
        m_Renderer->BeginFrame();

        m_Renderer->UseShaderCollection(m_MainShaderCollection);

        m_Renderer->BindVertexBuffers({m_TriangleVertexBuffer->GetBuffer().Get()}, VertexType::PositionColorUv, {0});
        m_Renderer->BindConstantBuffers({
                                            m_PerFrameConstantBuffer->GetBuffer().Get(),
                                            m_PerObjectConstantBuffer->GetBuffer().Get()
                                        },
                                        PipelineBindPoint::VertexShader);

        m_LinearSampler->UseSampler(0, PipelineBindPoint::PixelShader);

        if (m_UseFrogTexture) {
            m_FrogTexture->UseTexture(0, PipelineBindPoint::PixelShader);
        } else {
            m_FallbackTexture->UseTexture(0, PipelineBindPoint::PixelShader);
        }

        m_Renderer->Draw(3);

        m_Renderer->EndFrame();
    }

    void EngineApplication::OnKeyPressed(const KeyDownEvent& event) {
        if (Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift)) {
            switch (event.GetKeyCode()) {
            case Key::Escape:
                Close();
                break;
            case Key::T:
                m_UseFrogTexture = !m_UseFrogTexture;
                break;
            default:
                break;
            }
        }
    }
}
