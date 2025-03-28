// Copyright (C) 2025 Jean "Pixfri" Letessier
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#include <FlashlightEngine/Core/BaseObject.hpp>

#include <catch2/catch_test_macros.hpp>

class DaughterClass : public Fl::BaseObject {
public:
    DaughterClass();
    ~DaughterClass() override = default;
};

class GranddaughterClass final : public DaughterClass {
public:
    GranddaughterClass();
    ~GranddaughterClass() override = default;
};

SCENARIO("BaseObject", "[BaseObject]") {
    WHEN("Testing GetInfo") {
        const auto [name1, id1] = Fl::BaseObject::GetInfo<DaughterClass>();
        const auto [name2, id2] = Fl::BaseObject::GetInfo<GranddaughterClass>();

        CHECK(id1 != id2);
        CHECK(name1 != name2);

        CHECK(name1 == "DaughterClass");
        CHECK(name2 == "GranddaughterClass");
    }
}