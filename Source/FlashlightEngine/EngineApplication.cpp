// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#include <FlashlightEngine/EngineApplication.hpp>

#include <FlashlightEngine/Core/Input.hpp>
#include <FlashlightEngine/Core/KeyCodes.hpp>
#include <FlashlightEngine/Core/Filesystem.hpp>

namespace FlashlightEngine {
    EngineApplication::EngineApplication(const UInt32 width, const UInt32 height,
                                         const RendererValidationLevel validationLevel)
        : Application(width, height, "Flashlight Engine", validationLevel) {
    }

    EngineApplication::~EngineApplication() = default;

    void EngineApplication::OnEvent(Event& event) {
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<KeyDownEvent>(FL_BIND_EVENT(EngineApplication::OnKeyPressed));
    }

    void EngineApplication::OnUpdate() {
    }

    void EngineApplication::OnRender() {
        if (const auto commandBuffer = m_Renderer->BeginFrame()) {
            FlUnused(commandBuffer);



            constexpr VkClearColorValue clearValue = {{1.0f, 0.0f, 0.0f, 1.0f}};

            m_Renderer->BeginRendering(clearValue);

            m_Renderer->EndRendering();

            if (!m_Renderer->EndFrame()) {
            }
        }
    }

    void EngineApplication::OnKeyPressed(const KeyDownEvent& event) {
        if (Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift)) {
            switch (event.GetKeyCode()) {
            case Key::Escape:
                Close();
                break;
            case Key::F:
                GetWindow().SetFullscreen(!GetWindow().IsFullscreen());
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
