// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#include <FlashlightEngine/Core/Logger.hpp>
#include <FlashlightEngine/Core/Window.hpp>

int main() {
    FlashlightEngine::Logger::Init();

    const FlashlightEngine::WindowProperties properties{800, 600, "Flashlight Engine"};
    FlashlightEngine::Window window{properties};

    while (window.IsOpen()) {
        window.Update();
    }

    return EXIT_SUCCESS;
}
