// Copyright (C) 2025 Jean "Pixfri" Letessier
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#include <FlashlightEngine/Math/Angle.hpp>

#include <FlTests/CustomMatchers.hpp>

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Angle: Degrees to Radians", "[Math]") {
    constexpr Fl::Degreesf deg45(45.f);
    constexpr Fl::Degreesf deg90(90.f);
    constexpr Fl::Degreesf deg180(180.f);
    constexpr Fl::Degreesf deg360(360.f);

    CHECK_THAT(Fl::Radiansf(deg45).Value, IsNearlyEqualTo(Fl::Constants::Pi<float> / 4));
    CHECK_THAT(Fl::Radiansf(deg90).Value, IsNearlyEqualTo(Fl::Constants::Pi<float> / 2));
    CHECK_THAT(Fl::Radiansf(deg180).Value, IsNearlyEqualTo(Fl::Constants::Pi<float>));
    CHECK_THAT(Fl::Radiansf(deg360).Value, IsNearlyEqualTo(Fl::Constants::Pi<float> * 2));
}

TEST_CASE("Angle: Radians to Degrees", "[Math]") {
    constexpr Fl::Radiansf radFourthPi(Fl::Constants::Pi<float> / 4);
    constexpr Fl::Radiansf radHalfPi(Fl::Constants::Pi<float> / 2);
    constexpr Fl::Radiansf radPi(Fl::Constants::Pi<float>);
    constexpr Fl::Radiansf radTwoPi(Fl::Constants::Pi<float> * 2);

    CHECK_THAT(Fl::Degreesf(radFourthPi).Value, IsNearlyEqualTo(45.f));
    CHECK_THAT(Fl::Degreesf(radHalfPi).Value, IsNearlyEqualTo(90.f));
    CHECK_THAT(Fl::Degreesf(radPi).Value, IsNearlyEqualTo(180.f));
    CHECK_THAT(Fl::Degreesf(radTwoPi).Value, IsNearlyEqualTo(360.f));
}

TEST_CASE("Angle: Operations", "[Math]") {
    // Literal operators
    using namespace Fl::Literals;

    constexpr auto degTest = 180.0_deg;
    CHECK(std::is_same_v<Fl::Degreesld, std::decay_t<decltype(degTest)>>);

    constexpr auto radTest = 3.14159265358979323846_rad;
    CHECK(std::is_same_v<Fl::Radiansld, std::decay_t<decltype(radTest)>>);

    // Type conversion operators
    // To check conversion, verifying that values are (nearly) the same before & after conversion; in this example,
    // there won't be much difference
    // Degrees' & Radians' (un)equality operators automatically verify near-equality
    constexpr Fl::Degreesf degFloat = degTest;
    CHECK(degFloat == degTest);
    constexpr Fl::Degreesd degDouble = degTest;
    CHECK(degDouble == degTest);

    constexpr Fl::Radiansf radFloat = radTest;
    CHECK(radFloat == radTest);
    constexpr Fl::Radiansd radDouble = radTest;
    CHECK(radDouble == radTest);

    // Checking that conversion between degrees & radians with different data types works as expected
    constexpr Fl::Degreesf radToDegFloat = radTest;
    CHECK_THAT(radToDegFloat.Value, IsNearlyEqualTo(180.0f));

    constexpr Fl::Radiansf degToRadFloat = degTest;
    CHECK_THAT(degToRadFloat.Value, IsNearlyEqualTo(Fl::Constants::Pi<float>));

    // Mathematical operators
    // Degrees
    auto degAdd = degTest + 2.f;
    // Checking that mathematical operations keeps the original angle type (no implicit cast to the value type)
    CHECK(std::is_same_v<std::decay_t<decltype(degTest)>, std::decay_t<decltype(degAdd)>>);
    CHECK_THAT(degAdd.Value, IsNearlyEqualTo(182.0L));

    auto degSub = degTest - 2.f;
    CHECK(std::is_same_v<std::decay_t<decltype(degTest)>, std::decay_t<decltype(degSub)>>);
    CHECK_THAT(degSub.Value, IsNearlyEqualTo(178.0L));

    auto degMul = degTest * 2.f;
    CHECK(std::is_same_v<std::decay_t<decltype(degTest)>, std::decay_t<decltype(degMul)>>);
    CHECK_THAT(degMul.Value, IsNearlyEqualTo(360.0L));

    auto degDiv = degTest / 2.f;
    CHECK(std::is_same_v<std::decay_t<decltype(degTest)>, std::decay_t<decltype(degDiv)>>);
    CHECK_THAT(degDiv.Value, IsNearlyEqualTo(90.0L));

    degAdd -= 2.f;
    CHECK(degAdd == degTest);
    degSub += 2.f;
    CHECK(degSub == degTest);
    degMul /= 2.f;
    CHECK(degMul == degTest);
    degDiv *= 2.f;
    CHECK(degDiv == degTest);

    // Radians
    auto radAdd = radTest + Fl::Constants::Pi<float>;
    // Checking that mathematical operations keeps the original angle type (no implicit cast to the value type)
    CHECK(std::is_same_v<std::decay_t<decltype(radTest)>, std::decay_t<decltype(radAdd)>>);
    // A float epsilon must be specified for the test to pass
    // long double's epsilon is too precise, so we need to compare with a value of the type we made an operation with
    CHECK_THAT(radAdd.Value, IsNearlyEqualTo(Fl::Constants::Pi<long double> * 2, std::numeric_limits<float>::epsilon()));

    auto radSub = radTest - Fl::Constants::Pi<float>;
    CHECK(std::is_same_v<std::decay_t<decltype(radTest)>, std::decay_t<decltype(radSub)>>);
    CHECK_THAT(radSub.Value, IsNearlyEqualTo(0.0L, std::numeric_limits<float>::epsilon()));

    auto radMul = radTest * 2.f;
    CHECK(std::is_same_v<std::decay_t<decltype(radTest)>, std::decay_t<decltype(radMul)>>);
    CHECK_THAT(radMul.Value, IsNearlyEqualTo(Fl::Constants::Pi<long double> * 2, std::numeric_limits<float>::epsilon()));

    auto radDiv = radTest / 2.f;
    CHECK(std::is_same_v<std::decay_t<decltype(radTest)>, std::decay_t<decltype(radDiv)>>);
    CHECK_THAT(radDiv.Value, IsNearlyEqualTo(Fl::Constants::Pi<long double> / 2, std::numeric_limits<float>::epsilon()));

    radAdd -= Fl::Constants::Pi<float>;
    CHECK(radAdd == radTest);
    radSub += Fl::Constants::Pi<float>;
    CHECK(radSub == radTest);
    radMul /= 2.f;
    CHECK(radMul == radTest);
    radDiv *= 2.f;
    CHECK(radDiv == radTest);
}