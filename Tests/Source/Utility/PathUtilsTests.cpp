// Copyright (C) 2025 Jean "Pixfri" Letessier
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#include <FlashlightEngine/Utility/PathUtils.hpp>

#include <catch2/catch_test_macros.hpp>

SCENARIO("PathUtils", "[PathUtils]") {
    SECTION("Testing path functions") {
        CHECK(Fl::PathToString(Fl::Utf8Path("test")) == "test");
    }
}