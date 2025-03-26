// Copyright (C) 2025 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#include <FlashlightEngine/Utility/PathUtils.hpp>

namespace Fl {
    inline std::string PathToString(const std::filesystem::path& path) {
		// Android NDK 25c supports C++20 but its std::filesystem is not conforming (stills returns std::string)
#if FL_CHECK_CPP_VER(FL_CPP20) && (!defined(FL_PLATFORM_ANDROID) || FL_CHECK_NDK_VER(26))
        return FromUtf8String(path.generic_u8string());
#else
        return path.generic_u8string();
#endif
    }

    inline std::filesystem::path Utf8Path(std::string_view path) {
        // Android NDK 25c supports C++20 but its std::filesystem is not conforming
#if FL_CHECK_CPP_VER(FL_CPP20) && (!defined(FL_PLATFORM_ANDROID) || FL_CHECK_NDK_VER(26))
        return std::filesystem::path(ToUtf8String(path));
#else
        return std::filesystem::u8path(path);
#endif
    }
}
