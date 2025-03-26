// Copyright (C) 2025 Jean "Pixfri" Letessier
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#include <FlashlightEngine/Utility/StringUtils.hpp>

#include <catch2/catch_test_macros.hpp>

SCENARIO("StringUtils", "[StringUtils]") {
    using namespace std::literals;

    CHECK(Fl::FromUtf8String(u8"test") == "test");
    CHECK(Fl::FromUtf8String(u8"test"s) == "test");
    CHECK(Fl::FromUtf8String(u8"test"sv) == "test");
    CHECK(Fl::ToUtf8String("test") == u8"test");
    CHECK(Fl::ToUtf8String("test"s) == u8"test");
    CHECK(Fl::ToUtf8String("test"sv) == u8"test");
}