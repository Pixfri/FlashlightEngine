// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#include <FlashlightEngine/Core/Filesystem.hpp>

#include <spdlog/spdlog.h>

#include <fstream>

namespace FlashlightEngine {
    bool Filesystem::Exists(const std::filesystem::path& path) {
        return std::filesystem::exists(path);
    }

    bool Filesystem::IsDirectory(const std::filesystem::path& path) {
        return std::filesystem::is_directory(path);
    }

    void Filesystem::CreateFile(const std::filesystem::path& path) {
        // A trick to create a file is to create an empty file stream and close it directly.
        // std::filesystem has no function to create a file.
        std::ofstream tempFile(path);

        if (tempFile.bad()) {
            spdlog::error("[Filesystem] Failed to create file: {}", path.string());
            return;
        }

        tempFile.close();
    }

    void Filesystem::CreateDirectory(const std::filesystem::path& path) {
        if (!std::filesystem::create_directories(path)) {
            spdlog::error("[Filesystem] Failed to create directory: {}", path.string());
        }
    }

    void Filesystem::Delete(const std::filesystem::path& path) {
        if (!Exists(path)) {
            spdlog::warn("[Filesystem] Trying to delete file that does not exist: {}.", path.string());
            return;
        }

        if (!std::filesystem::remove(path)) {
            spdlog::error("[Filesystem] Failed to delete file: {}", path.string());
        }
    }

    void Filesystem::Move(const std::filesystem::path& from, const std::filesystem::path& to) {
        if (!Exists(from)) {
            spdlog::warn("[Filesystem] Trying to move file that does not exist: {}.", from.string());
            return;
        }

        std::filesystem::rename(from, to);
    }

    void Filesystem::Copy(const std::filesystem::path& from, const std::filesystem::path& to, const bool overwrite) {
        if (!Exists(from)) {
            spdlog::warn("[Filesystem] Trying to copy file that does not exist: {}.", from.string());
            return;
        }

        if (!std::filesystem::copy_file(from, to, overwrite
                                                      ? std::filesystem::copy_options::overwrite_existing
                                                      : std::filesystem::copy_options::none)) {
            spdlog::error("[Filesystem] Failed to copy file {} to {}", from.string(), to.string());
        }
    }

    std::string Filesystem::GetExtension(const std::filesystem::path& path) {
        return path.extension().string();
    }

    UInt64 Filesystem::GetFileSize(const std::filesystem::path& path) {
        return std::filesystem::file_size(path);
    }

    std::string Filesystem::ReadFile(const std::filesystem::path& path) {
        std::ifstream file(path, std::ios::ate);
        if (file.bad()) {
            spdlog::error("[Filesystem] Failed to open file: {}", path.string());
            return "";
        }

        const UInt64 size = GetFileSize(path);
        if (size == 0) {
            spdlog::error("[Filesystem] File is empty: {}", path.string());
            return "";
        }

        std::vector<char> buffer(size + 1);
        file.seekg(0);
        file.read(buffer.data(), size);

        buffer[size] = '\0';

        file.close();

        return std::string(buffer.data());
    }

    std::vector<char> Filesystem::ReadBytes(const std::filesystem::path& path) {
        std::ifstream file(path, std::ios::ate | std::ios::binary);
        if (file.bad()) {
            spdlog::error("[Filesystem] Failed to open file: {}", path.string());
            return {};
        }

        const UInt64 size = GetFileSize(path);
        if (size == 0) {
            spdlog::error("[Filesystem] File is empty: {}", path.string());
            return {};
        }

        std::vector<char> buffer(size);
        file.seekg(0);
        file.read(buffer.data(), size);

        return buffer;
    }
}
