// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#include <FlashlightEngine/EngineApplication.hpp>

#include <FlashlightEngine/Core/Input.hpp>
#include <FlashlightEngine/Core/KeyCodes.hpp>

namespace FlashlightEngine {
    EngineApplication::EngineApplication(const UInt32 width, const UInt32 height)
        : Application(width, height, "Flashlight Engine") {
        m_MainShaderCollection = m_Renderer->CreateShaderCollection(VertexType::PositionColor,
                                                                    "Resources/Shaders/Main.vs.hlsl",
                                                                    "Resources/Shaders/Main.ps.hlsl");
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
