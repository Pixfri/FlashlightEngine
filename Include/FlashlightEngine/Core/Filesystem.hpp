// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#ifndef FL_CORE_FILESYSTEM_HPP
#define FL_CORE_FILESYSTEM_HPP

#include <FlashlightEngine/Types.hpp>

#include <filesystem>

#include <FlashlightEngine/Libs/Windows.hpp>

namespace FlashlightEngine {
    class Filesystem {
    public:
        Filesystem() = delete;
        ~Filesystem() = delete;

        Filesystem(const Filesystem&) = delete;
        Filesystem(Filesystem&&) = delete;

        // File operations
        static bool Exists(const std::filesystem::path& path);
        static bool IsDirectory(const std::filesystem::path& path);

        static void CreateFile(const std::filesystem::path& path);
        static void CreateDirectory(const std::filesystem::path& path);

        static void Delete(const std::filesystem::path& path);
        static void Move(const std::filesystem::path& from, const std::filesystem::path& to);
        static void Copy(const std::filesystem::path& from, const std::filesystem::path& to, bool overwrite = true);

        static std::string GetExtension(const std::filesystem::path& path);

        static UInt64 GetFileSize(const std::filesystem::path& path);
        static std::string ReadFile(const std::filesystem::path& path);
        static std::vector<char> ReadBytes(const std::filesystem::path& path);

        // Directories
        static inline std::filesystem::path GetApplicationDirectory();
        static inline std::filesystem::path GetTempDirectory();
        static inline std::filesystem::path GetResourcesDirectory();
        static inline std::filesystem::path GetShadersDirectory();
        static inline std::filesystem::path GetTexturesDirectory();
        static inline std::filesystem::path GetModelsDirectory();
        static inline std::filesystem::path GetConfigDirectory();
        static inline std::filesystem::path GetLogsDirectory();

        Filesystem& operator=(const Filesystem&) = delete;
        Filesystem& operator=(Filesystem&&) = delete;
    
    private:
    };
}

#include <FlashlightEngine/Core/Filesystem.inl>

#endif // FL_CORE_FILESYSTEM_HPP
