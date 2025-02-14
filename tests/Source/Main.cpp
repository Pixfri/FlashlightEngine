// Copyright (C) 2025 Jean "Pixfri" Letessier
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#include <catch2/catch_session.hpp>
#include <catch2/catch_tostring.hpp>

int main(const int argc, char* argv[]) {
    Catch::StringMaker<float>::precision = std::numeric_limits<float>::digits10 + 3;
    Catch::StringMaker<double>::precision = std::numeric_limits<double>::digits10 + 3;

    return Catch::Session().run(argc, argv);
}
