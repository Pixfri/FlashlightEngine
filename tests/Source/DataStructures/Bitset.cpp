// Copyright (C) 2025 Jean "Pixfri" Letessier
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#include <FlashlightEngine/DataStructures/Bitset.hpp>

#include <catch2/catch_test_macros.hpp>

#include <sstream>

namespace {
    // Bitset to be tested
    const Fl::Bitset fullZeros(6, false); // 0 0 0 0 0 0
    const Fl::Bitset fullOnes(6, true); // 1 1 1 1 1 1

    const Fl::Bitset alternated1({true, false, true, false, true, false}); // 1 0 1 0 1 0
    const Fl::Bitset alternated2({false, true, false, true, false, true}); // 0 1 0 1 0 1
}

TEST_CASE("Bitset basic methods", "[Data Structures]") {
    CHECK(fullZeros.IsEmpty());
    CHECK_FALSE(fullOnes.IsEmpty());

    CHECK(fullZeros.GetSize() == 6);
    CHECK(fullOnes.GetSize() == fullZeros.GetSize());

    CHECK(fullZeros.GetEnabledBitCount() == 0);
    CHECK(fullZeros.GetDisabledBitCount() == 6);

    CHECK(fullOnes.GetEnabledBitCount() == 6);
    CHECK(fullOnes.GetDisabledBitCount() == 0);

    CHECK(alternated1.GetEnabledBitCount() == 3);
    CHECK(alternated2.GetEnabledBitCount() == 3);

    Fl::Bitset copy = fullOnes;
    CHECK(copy == fullOnes);

    copy.Resize(7);
    CHECK_FALSE(copy.GetSize() == fullOnes.GetSize());

    copy.Reset();
    CHECK(copy.IsEmpty());
}

TEST_CASE("Bitset manipulations", "[Data Structures]") {
    CHECK((alternated1 & alternated1) == alternated1);
    CHECK((alternated2 & alternated2) == alternated2);

    CHECK((alternated1 | alternated1) == alternated1);
    CHECK((alternated2 | alternated2) == alternated2);

    CHECK((alternated1 ^ alternated1) == fullZeros);
    CHECK((alternated2 ^ alternated2) == fullZeros);

    //     AND test
    //       ---
    //    1 0 1 0 1 0
    // &  0 1 0 1 0 1
    //   _____________
    // =  0 0 0 0 0 0
    CHECK((alternated1 & alternated2) == fullZeros);

    //      OR test
    //        ---
    //    1 0 1 0 1 0
    // |  0 1 0 1 0 1
    //   _____________
    // =  1 1 1 1 1 1
    CHECK((alternated1 | alternated2) == fullOnes);

    //     XOR test
    //       ---
    //    1 0 1 0 1 0
    // ^  0 1 0 1 0 1
    //   _____________
    // =  1 1 1 1 1 1
    CHECK((alternated1 ^ alternated2) == fullOnes);

    CHECK(~fullZeros == fullOnes);
    CHECK(~fullOnes == fullZeros);
    CHECK(~alternated1 == alternated2);
    CHECK(~alternated2 == alternated1);
}

TEST_CASE("Bitset shifts", "[Data Structures]") {
    CHECK((alternated1 << 1) == Fl::Bitset({true, false, true, false, true, false, false})); // 1 0 1 0 1 0 0
    CHECK((alternated1 >> 1) == Fl::Bitset({true, false, true, false, true})); // 1 0 1 0 1

    CHECK((alternated1 >> alternated1.GetSize()).GetSize() == 0);

    Fl::Bitset shiftTest = alternated1;
    shiftTest >>= 1; // 1 0 1 0 1 0 0
    shiftTest <<= 1; // 1 0 1 0 1 0

    CHECK(shiftTest == alternated1);
}

TEST_CASE("Bitset printing", "[Data Structures]") {
    std::stringstream stream;

    stream << fullOnes;
    CHECK(stream.str() == "[ 1, 1, 1, 1, 1, 1 ]");

    stream.str("");

    stream << alternated1;
    CHECK(stream.str() == "[ 1, 0, 1, 0, 1, 0 ]");
}