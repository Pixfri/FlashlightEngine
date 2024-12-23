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
        m_PipelineBuilder = m_Renderer->CreatePipelineBuilder();

        PipelineDescriptor mainPipelineDescriptor{};
        mainPipelineDescriptor.PipelineName = "MainPipeline";
        mainPipelineDescriptor.VertexType = VertexType::PositionColorUv;
        mainPipelineDescriptor.VertexShaderPath = "Main.vs.hlsl";
        mainPipelineDescriptor.PixelShaderPath = "Main.ps.hlsl";

        if (!m_PipelineBuilder->CreatePipeline(mainPipelineDescriptor, m_MainPipeline)) {
            spdlog::error("[Engine] Failed to create main pipeline.");
        }

        constexpr VertexPositionColorUv vertices[] = {
            //Front
            {Position{-0.5f, -0.5f, 0.5f}, Color{1.0f, 0.0f, 0.0f}, Uv{0.0f, 1.0f}},
            {Position{0.5f, -0.5f, 0.5f}, Color{0.0f, 1.0f, 0.0f}, Uv{1.0f, 1.0f}},
            {Position{-0.5f, 0.5f, 0.5f}, Color{0.0f, 0.0f, 1.0f}, Uv{0.0f, 0.0f}},
            {Position{0.5f, 0.5f, 0.5f}, Color{1.0f, 1.0f, 0.0f}, Uv{1.0f, 0.0f}},

            //Back
            {Position{-0.5f, -0.5f, -0.5f}, Color{0.0f, 1.0f, 1.0f}, Uv{0.0f, 1.0f}},
            {Position{0.5f, -0.5f, -0.5f}, Color{1.0f, 0.0f, 1.0f}, Uv{1.0f, 1.0f}},
            {Position{-0.5f, 0.5f, -0.5f}, Color{0.0f, 0.0f, 0.0f}, Uv{0.0f, 0.0f}},
            {Position{0.5f, 0.5f, -0.5f}, Color{1.0f, 1.0f, 1.0f}, Uv{1.0f, 0.0f}},
        };

        constexpr uint32_t indices[] =
        {
            //Top
            7, 6, 2,
            2, 3, 7,

            //Bottom
            0, 4, 5,
            5, 1, 0,

            //Left
            0, 2, 6,
            6, 4, 0,

            //Right
            7, 3, 1,
            1, 5, 7,

            //Front
            3, 2, 0,
            0, 1, 3,

            //Back
            4, 6, 7,
            7, 5, 4
        };

        m_CubeVertexBuffer = m_Renderer->CreateBuffer(vertices, sizeof(vertices), D3D11_USAGE_IMMUTABLE,
                                                      D3D11_BIND_VERTEX_BUFFER, "Cube Vertex Buffer");

        m_CubeIndexBuffer = m_Renderer->CreateBuffer(indices, sizeof(indices), D3D11_USAGE_IMMUTABLE,
                                                     D3D11_BIND_INDEX_BUFFER, "Cube Index Buffer");


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

    EngineApplication::~EngineApplication() = default;

    void EngineApplication::OnEvent(Event& event) {
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<KeyDownEvent>(FL_BIND_EVENT(EngineApplication::OnKeyPressed));
    }

    void EngineApplication::OnUpdate() {
        static Float32 yRotation = 0.0f;
        static Float32 scale = 1.0f;
        static DirectX::XMFLOAT3 cameraPosition = {0.0f, 0.0f, -1.0f};

        yRotation += static_cast<Float32>(Time::GetDeltaTime());

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

        m_MainPipeline->SetViewport(0, 0,
                                    static_cast<Float32>(GetWindow().GetWidth()),
                                    static_cast<Float32>(GetWindow().GetHeight()));
        m_MainPipeline->BindSampler(0, *m_LinearSampler, PipelineBindPoint::PixelShader);

        if (m_UseFrogTexture) {
            m_MainPipeline->BindTexture(0, *m_FrogTexture, PipelineBindPoint::PixelShader);
        } else {
            m_MainPipeline->BindTexture(0, *m_FallbackTexture, PipelineBindPoint::PixelShader);
        }

        m_MainPipeline->BindConstantBuffer(0, *m_PerFrameConstantBuffer, PipelineBindPoint::VertexShader);
        m_MainPipeline->BindConstantBuffer(1, *m_PerObjectConstantBuffer, PipelineBindPoint::VertexShader);

        m_Renderer->SetPipeline(m_MainPipeline.get());

        m_Renderer->SetVertexBuffer(*m_CubeVertexBuffer);
        m_Renderer->SetIndexBuffer(*m_CubeIndexBuffer);

        m_Renderer->DrawIndexed();

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
