// Copyright (C) 2025 Jean "Pixfri" Letessier
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#include <FlashlightEngine/Data/OwnerValue.hpp>

#include <catch2/catch_test_macros.hpp>

#include <limits>

TEST_CASE("OwnerValue: Move semantic", "[data]") {
    Fl::OwnerValue<int, std::numeric_limits<int>::max()> valLimit;
    CHECK(valLimit == std::numeric_limits<int>::max());
    CHECK_FALSE(valLimit.IsValid()); // If an initial value is unspecified, the invalid one is assigned

    Fl::OwnerValue<int> valZero;
    CHECK(valZero == 0); // If unspecified, the default invalid value is default-initialized
    CHECK_FALSE(valZero.IsValid());

    valLimit = 3;
    CHECK(valLimit.IsValid()); // The given value is not equal to the invalid one, thus making it valid

    Fl::OwnerValue valMoveCtor(std::move(valLimit));
    CHECK(valMoveCtor == 3); // The moved-to value has taken the moved-from's
    CHECK(valLimit == std::numeric_limits<int>::max()); // The moved-from value is now invalid
    CHECK_FALSE(valLimit.IsValid());

    valZero = 10;
    CHECK(valZero.IsValid());

    Fl::OwnerValue valMoveOp(1);
    valMoveOp = std::move(valZero);
    CHECK(valMoveOp == 10); // The values have been swapped
    CHECK(valZero == 1);
}