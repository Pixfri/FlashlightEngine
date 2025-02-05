// Copyright (C) 2025 Jean "Pixfri" Letessier
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#include <FlashlightEngine/DataStructures/Bitset.hpp>

#include <catch2/catch_test_macros.hpp>

#include <sstream>

namespace {
    // Bitset to be tested
    const Fl::Bitset g_FullZeros(6, false); // 0 0 0 0 0 0
    const Fl::Bitset g_FullOnes(6, true); // 1 1 1 1 1 1

    const Fl::Bitset g_Alternated1({true, false, true, false, true, false}); // 1 0 1 0 1 0
    const Fl::Bitset g_Alternated2({false, true, false, true, false, true}); // 0 1 0 1 0 1
}

TEST_CASE("Bitset: Basic methods", "[Data Structures]") {
    CHECK(g_FullZeros.IsEmpty());
    CHECK_FALSE(g_FullOnes.IsEmpty());

    CHECK(g_FullZeros.GetSize() == 6);
    CHECK(g_FullOnes.GetSize() == g_FullZeros.GetSize());

    CHECK(g_FullZeros.GetEnabledBitCount() == 0);
    CHECK(g_FullZeros.GetDisabledBitCount() == 6);

    CHECK(g_FullOnes.GetEnabledBitCount() == 6);
    CHECK(g_FullOnes.GetDisabledBitCount() == 0);

    CHECK(g_Alternated1.GetEnabledBitCount() == 3);
    CHECK(g_Alternated2.GetEnabledBitCount() == 3);

    Fl::Bitset copy = g_FullOnes;
    CHECK(copy == g_FullOnes);

    copy.Resize(7);
    CHECK_FALSE(copy.GetSize() == g_FullOnes.GetSize());

    copy.Reset();
    CHECK(copy.IsEmpty());
}

TEST_CASE("Bitset: Bitwise manipulations", "[Data Structures]") {
    CHECK((g_Alternated1 & g_Alternated1) == g_Alternated1);
    CHECK((g_Alternated2 & g_Alternated2) == g_Alternated2);

    CHECK((g_Alternated1 | g_Alternated1) == g_Alternated1);
    CHECK((g_Alternated2 | g_Alternated2) == g_Alternated2);

    CHECK((g_Alternated1 ^ g_Alternated1) == g_FullZeros);
    CHECK((g_Alternated2 ^ g_Alternated2) == g_FullZeros);

    //     AND test
    //       ---
    //    1 0 1 0 1 0
    // &  0 1 0 1 0 1
    //   _____________
    // =  0 0 0 0 0 0
    CHECK((g_Alternated1 & g_Alternated2) == g_FullZeros);

    //      OR test
    //        ---
    //    1 0 1 0 1 0
    // |  0 1 0 1 0 1
    //   _____________
    // =  1 1 1 1 1 1
    CHECK((g_Alternated1 | g_Alternated2) == g_FullOnes);

    //     XOR test
    //       ---
    //    1 0 1 0 1 0
    // ^  0 1 0 1 0 1
    //   _____________
    // =  1 1 1 1 1 1
    CHECK((g_Alternated1 ^ g_Alternated2) == g_FullOnes);

    CHECK(~g_FullZeros == g_FullOnes);
    CHECK(~g_FullOnes == g_FullZeros);
    CHECK(~g_Alternated1 == g_Alternated2);
    CHECK(~g_Alternated2 == g_Alternated1);
}

TEST_CASE("Bitset: Shifting operations", "[Data Structures]") {
    CHECK((g_Alternated1 << 1) == Fl::Bitset({true, false, true, false, true, false, false})); // 1 0 1 0 1 0 0
    CHECK((g_Alternated1 >> 1) == Fl::Bitset({true, false, true, false, true})); // 1 0 1 0 1

    CHECK((g_Alternated1 >> g_Alternated1.GetSize()).GetSize() == 0);

    Fl::Bitset shiftTest = g_Alternated1;
    shiftTest >>= 1; // 1 0 1 0 1 0 0
    shiftTest <<= 1; // 1 0 1 0 1 0

    CHECK(shiftTest == g_Alternated1);
}

TEST_CASE("Bitset: Printing", "[Data Structures]") {
    std::stringstream stream;

    stream << g_FullOnes;
    CHECK(stream.str() == "[ 1, 1, 1, 1, 1, 1 ]");

    stream.str("");

    stream << g_Alternated1;
    CHECK(stream.str() == "[ 1, 0, 1, 0, 1, 0 ]");
}