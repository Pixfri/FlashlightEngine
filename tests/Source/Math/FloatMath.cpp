// Copyright (C) 2025 Jean "Pixfri" Letessier
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#include <FlashlightEngine/Math/FloatMath.hpp>

#include <catch2/catch_test_macros.hpp>

TEST_CASE("FloatMath: Near-equality test", "[Math]") {
    constexpr float floatEpsilon = std::numeric_limits<float>::epsilon();
    constexpr double doubleEpsilon = std::numeric_limits<double>::epsilon();
    constexpr long double longDoubleEpsilon = std::numeric_limits<long double>::epsilon();

    // An epsilon difference is considered nearly equal
    CHECK(Fl::FloatMath::AreNearlyEqual(0.f, floatEpsilon));
    CHECK(Fl::FloatMath::AreNearlyEqual(0.0, doubleEpsilon));
    CHECK(Fl::FloatMath::AreNearlyEqual(0.0L, longDoubleEpsilon));

    // A two-epsilon difference is however not considered nearly equal
    CHECK_FALSE(Fl::FloatMath::AreNearlyEqual(0.f, floatEpsilon * 2.f));
    CHECK_FALSE(Fl::FloatMath::AreNearlyEqual(0.0, doubleEpsilon * 2.0));
    CHECK_FALSE(Fl::FloatMath::AreNearlyEqual(0.0L, longDoubleEpsilon * 2.0L));
}