// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#ifndef FL_UTILS_FILEUTILS_HPP
#define FL_UTILS_FILEUTILS_HPP

#include <FlashlightEngine/Types.hpp>

#include <expected>
#include <string>
#include <string_view>

namespace FlashlightEngine {
    std::expected<std::string, std::string> ReadFile(std::string_view path);
}

#include <FlashlightEngine/Utils/FileUtils.inl>

#endif // FL_UTILS_FILEUTILS_HPP
