// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#include <FlashlightEngine/Utils/FileUtils.hpp>

#include <fstream>

namespace FlashlightEngine {
    std::expected<std::string, std::string> ReadFile(const std::string_view path) {
        std::ifstream file(path.data(), std::ios::ate);
        if (file.bad()) {
            return std::unexpected(std::format("[IO] Unable to read file {}", path));
        }
        const auto fileSize = file.tellg();
        if (fileSize == 0) {
            return std::unexpected(std::format("[IO] File {} is empty", path));
        }

        std::string content(fileSize, '\0');
        file.seekg(0);
        file.read(content.data(), static_cast<std::streamsize>(content.size()));
        return content;
    }
}
