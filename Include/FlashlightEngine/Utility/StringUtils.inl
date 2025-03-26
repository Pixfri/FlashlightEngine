// Copyright (C) 2025 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#include <FlashlightEngine/Utility/StringUtils.hpp>

namespace Fl {
#if FL_CHECK_CPP_VER(FL_CPP20)
    inline std::string_view FromUtf8String(const char8_t* str) {
        return std::string_view(reinterpret_cast<const char*>(str));
    }

    inline std::string FromUtf8String(const std::u8string& str) {
        return std::string(reinterpret_cast<const char*>(str.data()), str.size());
    }

    inline std::string_view FromUtf8String(const std::u8string_view str) {
        return std::string_view(reinterpret_cast<const char*>(str.data()), str.size());
    }
#else
    inline std::string_view FromUtf8String(const char* str) {
        return str;
    }

    inline std::string FromUtf8String(const std::string& str) {
        return str;
    }

    inline std::string_view FromUtf8String(const std::string_view& str) {
        return str;
    }
#endif

#if FL_CHECK_CPP_VER(FL_CPP20)
    inline std::u8string_view ToUtf8String(const char* str) {
        return ToUtf8String(std::string_view(str));
    }

    inline std::u8string ToUtf8String(const std::string& str) {
        return std::u8string(reinterpret_cast<const char8_t*>(str.data()), str.size());
    }

    inline std::u8string_view ToUtf8String(const std::string_view str) {
        return std::u8string_view(reinterpret_cast<const char8_t*>(str.data()), str.size());
    }
#else
    inline std::string ToUtf8String(const char* str) {
        return str;
    }

    inline std::string ToUtf8String(std::string str) {
        return str;
    }

    inline std::string_view ToUtf8String(std::string_view str) {
        return str;
    }
#endif
}
