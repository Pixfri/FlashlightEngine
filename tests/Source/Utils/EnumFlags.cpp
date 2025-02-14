// Copyright (C) 2025 Jean "Pixfri" Letessier
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#include <FlashlightEngine/Utils/EnumFlags.hpp>

#include <catch2/catch_test_macros.hpp>

enum class EnumTest : unsigned int {
    One   = Fl::Bit(0),
    Two   = Fl::Bit(1),
    Four  = Fl::Bit(2),
    Eight = Fl::Bit(3),
    All   = One | Two | Four | Eight // 15
  };

FL_MAKE_ENUM_FLAG(EnumTest)

TEST_CASE("EnumUtils make enum flag", "[utils]") {
    constexpr EnumTest zero   = EnumTest::One & EnumTest::Two;
    constexpr EnumTest three  = EnumTest::One | EnumTest::Two;
    constexpr EnumTest one    = three & EnumTest::One;
    constexpr EnumTest twelve = EnumTest::Four ^ EnumTest::Eight;

    CHECK(static_cast<unsigned int>(zero) == 0);
    CHECK(static_cast<unsigned int>(three) == 3);
    CHECK(static_cast<unsigned int>(one) == 1);
    CHECK(static_cast<unsigned int>(twelve) == 12);

    EnumTest five = EnumTest::Four;
    five |= EnumTest::One;
    CHECK(static_cast<unsigned int>(five) == 5);

    EnumTest four = five;
    four &= EnumTest::Four;
    CHECK(static_cast<unsigned int>(four) == 4);

    EnumTest six = EnumTest::Two;
    six ^= EnumTest::Four;
    CHECK(static_cast<unsigned int>(six) == 6);

    CHECK(static_cast<unsigned int>(EnumTest::All & EnumTest::One) == 1);
    CHECK(static_cast<unsigned int>(EnumTest::All & EnumTest::Two) == 2);
    CHECK(static_cast<unsigned int>(EnumTest::All & EnumTest::Four) == 4);
    CHECK(static_cast<unsigned int>(EnumTest::All & EnumTest::Eight) == 8);

    CHECK(static_cast<unsigned int>(EnumTest::All | EnumTest::One) == 15);
    CHECK(static_cast<unsigned int>(EnumTest::All | EnumTest::Two) == 15);
    CHECK(static_cast<unsigned int>(EnumTest::All | EnumTest::Four) == 15);
    CHECK(static_cast<unsigned int>(EnumTest::All | EnumTest::Eight) == 15);

    CHECK(static_cast<unsigned int>(EnumTest::All ^ EnumTest::One) == 14);
    CHECK(static_cast<unsigned int>(EnumTest::All ^ EnumTest::Two) == 13);
    CHECK(static_cast<unsigned int>(EnumTest::All ^ EnumTest::Four) == 11);
    CHECK(static_cast<unsigned int>(EnumTest::All ^ EnumTest::Eight) == 7);
}