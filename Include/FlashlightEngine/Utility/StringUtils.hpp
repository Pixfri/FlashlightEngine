// Copyright (C) 2025 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#ifndef FL_UTILITY_STRINGUTILS_HPP
#define FL_UTILITY_STRINGUTILS_HPP

#include <FlashlightEngine/Prerequisites.hpp>

#include <string>
#include <string_view>

namespace Fl {
#if FL_CHECK_CPP_VER(FL_CPP20)
    inline std::string_view FromUtf8String(const char8_t* str);
    inline std::string FromUtf8String(const std::u8string& str);
    inline std::string_view FromUtf8String(std::u8string_view str);
#else
    inline std::string_view FromUtf8String(const char* str);
    inline std::string FromUtf8String(const std::string& str);
    inline std::string_view FromUtf8String(const std::string_view& str);
#endif

#if FL_CHECK_CPP_VER(FL_CPP20)
    inline std::u8string_view ToUtf8String(const char* str);
    inline std::u8string ToUtf8String(const std::string& str);
    inline std::u8string_view ToUtf8String(std::string_view str);
#else
    inline std::string ToUtf8String(const char* str);
    inline std::string ToUtf8String(std::string str);
    inline std::string_view ToUtf8String(std::string_view str);
#endif
}

#include <FlashlightEngine/Utility/StringUtils.inl>

#endif // FL_UTILITY_STRINGUTILS_HPP
