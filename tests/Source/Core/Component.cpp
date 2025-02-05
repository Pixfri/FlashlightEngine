// Copyright (C) 2025 Jean "Pixfri" Letessier
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#include <FlashlightEngine/Core/Component.hpp>

#include <catch2/catch_test_macros.hpp>

namespace {
    struct FirstTestComponent final : public Fl::Component {
    };

    struct SecondTestComponent final : public Fl::Component {
    };

    struct ThirdTestComponent final : public Fl::Component {
    };

    struct ForthTestComponent final : public Fl::Component {
    };
}

TEST_CASE("Component: IDs", "[Core/ECS]") {
    // With the CRTP, every component gets a different constant ID with the first call
    // The ID is incremented with every distinct component call
    // No matter how many times one component is checked, it will always have the same ID

    const Fl::U64 firstIndex = Fl::Component::GetId<FirstTestComponent>();
    const Fl::U64 secondIndex = Fl::Component::GetId<SecondTestComponent>();
    const Fl::U64 thirdIndex = Fl::Component::GetId<ThirdTestComponent>();
    const Fl::U64 forthIndex = Fl::Component::GetId<ForthTestComponent>();

    CHECK(firstIndex == Fl::Component::GetId<FirstTestComponent>());
    CHECK(secondIndex == Fl::Component::GetId<SecondTestComponent>());
    CHECK(thirdIndex == Fl::Component::GetId<ThirdTestComponent>());
    CHECK(forthIndex == Fl::Component::GetId<ForthTestComponent>());
}
