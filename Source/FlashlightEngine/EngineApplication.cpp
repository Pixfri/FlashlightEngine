// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#include <FlashlightEngine/EngineApplication.hpp>

#include <FlashlightEngine/Core/Input.hpp>
#include <FlashlightEngine/Core/KeyCodes.hpp>
#include <FlashlightEngine/Core/Filesystem.hpp>

namespace FlashlightEngine {
    EngineApplication::EngineApplication(const UInt32 width, const UInt32 height)
        : Application(width, height, "Flashlight Engine <Direct3D 11>") {
        m_MainShaderCollection = m_Renderer->CreateShaderCollection(
            VertexType::PositionColorUv,
            Filesystem::GetShadersDirectory() / "Main.vs.hlsl",
            Filesystem::GetShadersDirectory() / "Main.ps.hlsl"
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
                                                          D3D11_BIND_VERTEX_BUFFER);

        m_LinearSampler = m_Renderer->CreateSampler(D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT);

        m_FallbackTexture = m_Renderer->CreateTexture(Filesystem::GetTexturesDirectory() / "Default.png");
        m_FrogTexture = m_Renderer->CreateTexture(Filesystem::GetTexturesDirectory() / "T_Froge.dds");

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
    }

    void EngineApplication::OnRender() {
        m_Renderer->BeginFrame();

        m_Renderer->UseShaderCollection(m_MainShaderCollection);

        m_Renderer->BindVertexBuffer(*m_TriangleVertexBuffer, VertexType::PositionColorUv);

        m_LinearSampler->UseSampler(0);

        if (m_UseFrogTexture) {
            m_FrogTexture->UseTexture(0);
        } else {
            m_FallbackTexture->UseTexture(0);
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
