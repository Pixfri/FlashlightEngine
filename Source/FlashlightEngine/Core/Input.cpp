// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#include <FlashlightEngine/Core/Input.hpp>

#include <FlashlightEngine/Application.hpp>

#include <GLFW/glfw3.h>

namespace FlashlightEngine {
    static GLFWwindow* GetWindow() {
        return Application::Get().GetWindow().GetNativeWindow();
    }

    bool Input::IsKeyPressed(const Int32 keyCode) {
        const auto state = glfwGetKey(GetWindow(), keyCode);
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool Input::IsMouseButtonPressed(const Int32 button) {
        return glfwGetMouseButton(GetWindow(), button) == GLFW_PRESS;
    }

    void Input::GetMousePos(Float64& x, Float64& y) {
        glfwGetCursorPos(GetWindow(), &x, &y);
    }

    Float64 Input::GetMouseX() {
        Float64 x = 0, y = 0;
        GetMousePos(x, y);

        return x;
    }

    Float64 Input::GetMouseY() {
        Float64 x = 0, y = 0;
        GetMousePos(x, y);

        return y;
    }
}
