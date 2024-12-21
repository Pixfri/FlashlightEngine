// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#include <FlashlightEngine/EngineApplication.hpp>

#include <FlashlightEngine/Core/Input.hpp>
#include <FlashlightEngine/Core/KeyCodes.hpp>

namespace FlashlightEngine {
    EngineApplication::EngineApplication(const UInt32 width, const UInt32 height)
        : Application(width, height, "Flashlight Engine <Direct3D 11>") {
        m_MainShaderCollection = m_Renderer->CreateShaderCollection(VertexType::PositionColor,
                                                                    "Resources/Shaders/Main.vs.hlsl",
                                                                    "Resources/Shaders/Main.ps.hlsl"
        );

        constexpr VertexPositionColor vertices[] = {
            {Position{0.0f, 0.5f, 0.0f}, Color{0.25f, 0.39f, 0.19f}},
            {Position{0.5f, -0.5f, 0.0f}, Color{0.44f, 0.75f, 0.35f}},
            {Position{-0.5f, -0.5f, 0.0f}, Color{0.38f, 0.55f, 0.20f}},
        };

        m_TriangleVertexBuffer = m_Renderer->CreateBuffer(vertices, sizeof(vertices), D3D11_USAGE_IMMUTABLE,
                                                          D3D11_BIND_VERTEX_BUFFER);
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

        m_Renderer->BindVertexBuffer(*m_TriangleVertexBuffer, VertexType::PositionColor);

        m_Renderer->Draw(3);

        m_Renderer->EndFrame();
    }

    void EngineApplication::OnKeyPressed(const KeyDownEvent& event) {
        if (Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift)) {
            switch (event.GetKeyCode()) {
            case Key::Escape:
                Close();
                break;
            case Key::V:
                GetWindow().SetVSync(!GetWindow().IsVSync());
                break;
            default:
                break;
            }
        }
    }
}
