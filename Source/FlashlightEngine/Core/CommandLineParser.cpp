// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#include <FlashlightEngine/Core/CommandLineParser.hpp>

#include <algorithm>

namespace FlashlightEngine {
    CommandLineParser::CommandLineParser(const Int32 argc, char* argv[]) {
        for (UInt32 i = 1; i < static_cast<UInt32>(argc); ++i) {
            m_Tokens.push_back(std::string(argv[i]));
        }
    }

    std::string CommandLineParser::GetCmdOption(const std::string& option) const {
        if (auto it = std::ranges::find(m_Tokens, option);
            it != m_Tokens.end() && ++it != m_Tokens.end()) {
            return *it;
        }

        static const std::string emptyString("");
        return emptyString;
    }

    bool CommandLineParser::CmdOptionExists(const std::string& option) const {
        return std::ranges::find(m_Tokens, option) != m_Tokens.end();
    }
}
