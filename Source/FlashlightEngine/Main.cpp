// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#include <FlashlightEngine/Types.hpp>

#include <FlashlightEngine/EngineApplication.hpp>
#include <FlashlightEngine/Core/CommandLineParser.hpp>
#include <FlashlightEngine/Core/Filesystem.hpp>

#include <spdlog/spdlog.h>
#include <spdlog/cfg/argv.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

#include <cstdlib>
#include <iostream>
#include <stdexcept>

FlashlightEngine::UInt32 g_Width = 1280;
FlashlightEngine::UInt32 g_Height = 720;
bool g_UseHighPerfGPU = true;

void SetupLogger(int argc, char* argv[]);
void ParseArguments(int argc, char* argv[]);

int main(const int argc, char* argv[]) {
    try {
        SetupLogger(argc, argv);
        ParseArguments(argc, argv);

        FlashlightEngine::EngineApplication app(g_Width, g_Height, g_UseHighPerfGPU);

        app.Run();
    } catch (const std::runtime_error& e) {
        std::cerr << "An error occurred while running the application: " << e.what() << std::endl;
        return EXIT_FAILURE;
    } catch (const spdlog::spdlog_ex& e) {
        std::cerr << "Failed to initialize logger: " << e.what() << std::endl;
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

    // Set the default mode depending on the build mode.
#if defined(FL_DEBUG)
    spdlog::set_level(spdlog::level::debug);
#else
    spdlog::set_level(spdlog::level::info);
#endif

    spdlog::set_pattern("[%H:%M:%S %z] [%^---%L---%$] [thread %t] %v");

    spdlog::cfg::load_argv_levels(argc, argv);
}

void ParseArguments(const int argc, char* argv[]) {
    const FlashlightEngine::CommandLineParser cmdLineParser(argc, argv);
    if (cmdLineParser.CmdOptionExists("--width")) {
        if (const std::string width = cmdLineParser.GetCmdOption("--width");
            width.empty()) {
            spdlog::warn("No width value provided after --width. Keeping the default value of {} instead.",
                         g_Width);
        } else {
            g_Width = static_cast<FlashlightEngine::UInt32>(std::stoi(width));
        }
    }

    if (cmdLineParser.CmdOptionExists("--height")) {
        if (const std::string height = cmdLineParser.GetCmdOption("--height");
            height.empty()) {
            spdlog::warn("No height value provided after --height. Keeping the default value of {} instead.",
                         g_Height);
        } else {
            g_Height = static_cast<FlashlightEngine::UInt32>(std::stoi(height));
        }
    }

    if (cmdLineParser.CmdOptionExists("--highperf")) {
        g_UseHighPerfGPU = true;
    }

    if (cmdLineParser.CmdOptionExists("--lowperf")) {
        g_UseHighPerfGPU = false;
    }
}
