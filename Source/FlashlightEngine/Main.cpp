// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#include <FlashlightEngine/Types.hpp>

#include <FlashlightEngine/EngineApplication.hpp>

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <stdexcept>


int main(const int argc, char* argv[]) {
    try {
        FlashlightEngine::UInt32 width = 1280;
        FlashlightEngine::UInt32 height = 720;

        // If argc (argument count) is one, it means the program was run without arguments
        // (argv[0] = the name of the program, hence argc = 1 when there are no arguments).
        if (argc > 1) {
            for (FlashlightEngine::Int32 i = 1; i < argc - 1; i++) {
                if (std::strcmp(argv[i], "--width") == 0 || std::strcmp(argv[i], "-w") == 0) {
                    char* end;
                    const long value = std::strtol(argv[i + 1], &end, 10);

                    if (*end != '\0' || value <= 0) {
                        throw std::invalid_argument("Invalid width value provided after --width.");
                    }

                    width = static_cast<FlashlightEngine::UInt32>(value);
                }

                if (std::strcmp(argv[i], "--height") == 0 || std::strcmp(argv[i], "-h") == 0) {
                    char* end;
                    const long value = std::strtol(argv[i + 1], &end, 10);

                    if (*end != '\0' || value <= 0) {
                        throw std::invalid_argument("Invalid height value provided after --height.");
                    }

                    height = static_cast<FlashlightEngine::UInt32>(value);
                }
            }
        }

        FlashlightEngine::EngineApplication app(width, height);

        app.Run();

    } catch (const std::invalid_argument& e) {
        std::cerr << "Invalid argument: " << e.what() << std::endl;
        return EXIT_FAILURE;
    } catch (const std::runtime_error& e) {
        std::cerr << "An error occurred while running the application: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
