// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#include <FlashlightEngine/Types.hpp>

#include <FlashlightEngine/EngineApplication.hpp>
#include <FlashlightEngine/Core/Filesystem.hpp>

#include <spdlog/spdlog.h>
#include <spdlog/cfg/argv.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <stdexcept>

FlashlightEngine::UInt32 g_Width = 1280;
FlashlightEngine::UInt32 g_Height = 720;

void SetupLogger(int argc, char* argv[]);
void ParseArguments(int argc, char* argv[]);

int main(const int argc, char* argv[]) {
    try {
        SetupLogger(argc, argv);
        ParseArguments(argc, argv);

        FlashlightEngine::EngineApplication app(g_Width, g_Height);

        app.Run();
    } catch (const std::invalid_argument& e) {
        std::cerr << "Invalid argument: " << e.what() << std::endl;
        return EXIT_FAILURE;
    } catch (const std::runtime_error& e) {
        std::cerr << "An error occurred while running the application: " << e.what() << std::endl;
        return EXIT_FAILURE;
    } catch (const spdlog::spdlog_ex& ex) {
        std::cerr << "Failed to initialize logger: " << ex.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

void SetupLogger(const int argc, char* argv[]) {
    const auto logFile = FlashlightEngine::Filesystem::GetLogsDirectory() / "FlashlightEngine.log";

    std::vector<spdlog::sink_ptr> sinks;
    sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
    sinks.push_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>(logFile.string(), true));

    const auto logger = std::make_shared<spdlog::logger>("FlashlightEngine", begin(sinks), end(sinks));
    spdlog::register_logger(logger);
    spdlog::set_default_logger(logger);

    spdlog::set_pattern("[%H:%M:%S %z] [%^---%L---%$] [thread %t] %v");

    spdlog::cfg::load_argv_levels(argc, argv);
}

void ParseArguments(const int argc, char* argv[]) {
    // If argc (argument count) is one, it means the program was run without arguments
    // (argv[0] = the name of the program, hence argc = 1 when there are no arguments).
    if (argc > 1) {
        for (FlashlightEngine::Int32 i = 1; i < argc; i++) {
            if (std::strcmp(argv[i], "--width") == 0 || std::strcmp(argv[i], "-w") == 0) {
                char* end;
                const long value = std::strtol(argv[i + 1], &end, 10);

                if (*end != '\0' || value <= 0) {
                    throw std::invalid_argument("Invalid width value provided after --width.");
                }

                g_Width = static_cast<FlashlightEngine::UInt32>(value);
            }

            if (std::strcmp(argv[i], "--height") == 0 || std::strcmp(argv[i], "-h") == 0) {
                char* end;
                const long value = std::strtol(argv[i + 1], &end, 10);

                if (*end != '\0' || value <= 0) {
                    throw std::invalid_argument("Invalid height value provided after --height.");
                }

                g_Height = static_cast<FlashlightEngine::UInt32>(value);
            }
        }
    }
}