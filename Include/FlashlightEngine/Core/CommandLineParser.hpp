// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#ifndef FL_CORE_COMMANDLINEPARSER_HPP
#define FL_CORE_COMMANDLINEPARSER_HPP

#include <FlashlightEngine/Types.hpp>

#include <string>
#include <vector>

namespace FlashlightEngine {
    // From https://stackoverflow.com/questions/865668/parsing-command-line-arguments-in-c?page=1&tab=scoredesc#tab-top
    class CommandLineParser {
    public:
        CommandLineParser(Int32 argc, char* argv[]);
        ~CommandLineParser() = default;

        CommandLineParser(const CommandLineParser&) = delete;
        CommandLineParser(CommandLineParser&&) = delete;

        std::string GetCmdOption(const std::string& option) const;

        bool CmdOptionExists(const std::string& option) const;

        CommandLineParser& operator=(const CommandLineParser&) = delete;
        CommandLineParser& operator=(CommandLineParser&&) = delete;
    
    private:
        std::vector<std::string> m_Tokens;
    };
}

#include <FlashlightEngine/Core/CommandLineParser.inl>

#endif // FL_CORE_COMMANDLINEPARSER_HPP
