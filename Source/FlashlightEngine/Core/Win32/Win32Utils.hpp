// Copyright (C) 2025 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#ifndef FL_CORE_WIN32_WIN32UTILS_HPP
#define FL_CORE_WIN32_WIN32UTILS_HPP

#include <FlashlightEngine/Prerequisites.hpp>
#include <FlashlightEngine/Core/BaseObject.hpp>

#include <Windows.h>

#include <ctime>
#include <filesystem>

namespace Fl::PlatformImpl {
    constexpr bool ArePathsWide = std::is_same_v<std::filesystem::path::value_type, wchar_t>;

    using WidePathHolder = std::conditional_t<ArePathsWide, std::wstring_view, std::wstring>;

    inline WidePathHolder PathToWideTemp(const std::filesystem::path& path);
    time_t FileTimeToTime(FILETIME* time);
}

#include <FlashlightEngine/Core/Win32/Win32Utils.inl>

#endif // FL_CORE_WIN32_WIN32UTILS_HPP
