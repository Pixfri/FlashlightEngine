// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

namespace FlashlightEngine {
    inline std::filesystem::path Filesystem::GetApplicationDirectory() {
        return std::filesystem::current_path();
    }

    inline std::filesystem::path Filesystem::GetTempDirectory() {
        return std::filesystem::temp_directory_path();
    }

    inline std::filesystem::path Filesystem::GetResourcesDirectory() {
        return GetApplicationDirectory() / "Resources";
    }

    inline std::filesystem::path Filesystem::GetShadersDirectory() {
        return GetResourcesDirectory() / "Shaders";
    }

    inline std::filesystem::path Filesystem::GetTexturesDirectory() {
        return GetResourcesDirectory() / "Textures";
    }

    inline std::filesystem::path Filesystem::GetModelsDirectory() {
        return GetResourcesDirectory() / "Models";
    }

    inline std::filesystem::path Filesystem::GetConfigDirectory() {
        return GetApplicationDirectory() / "Config";
    }

    inline std::filesystem::path Filesystem::GetLogsDirectory() {
        return GetApplicationDirectory() / "Logs";
    }
}
