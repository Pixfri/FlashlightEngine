// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#include <FlashlightEngine/Core/Logger.hpp>

int main() {
    FlashlightEngine::Logger::Init();

    FlashlightEngine::Log::Trace("Hello Trace!");
    FlashlightEngine::Log::Debug("Hello Debug!");
    FlashlightEngine::Log::Info("Hello Info!");
    FlashlightEngine::Log::Warn("Hello Warn!");
    FlashlightEngine::Log::Error("Hello Error!");
    FlashlightEngine::Log::Critical("Hello Critical!");

    return EXIT_SUCCESS;
}
