// Copyright (C) 2025 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#ifndef FL_UTILITY_PATHUTILS_HPP
#define FL_UTILITY_PATHUTILS_HPP

#include <FlashlightEngine/Prerequisites.hpp>
#include <FlashlightEngine/Utility/StringUtils.hpp>

#include <filesystem>
#include <string>

namespace Fl {
    [[nodiscard]] inline std::string PathToString(const std::filesystem::path& path);
    [[nodiscard]] inline std::filesystem::path Utf8Path(std::string_view path);
}

#include <FlashlightEngine/Utility/PathUtils.inl>

#endif // FL_UTILITY_PATHUTILS_HPP
