// Copyright (C) 2025 Jean "Pixfri" Letessier
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#include <FlashlightEngine/Core/Component.hpp>
#include <FlashlightEngine/Core/Entity.hpp>

#include <catch2/catch_test_macros.hpp>

namespace {
    struct FirstTestComponent final : public Fl::Component {
    };

    struct SecondTestComponent final : public Fl::Component {
    };
}

TEST_CASE("Entity: Basic", "[Core/ECS]") {
    Fl::Entity entity0(0);
    const Fl::Entity entity1(1);
    const Fl::Entity entity2(2, false);

    CHECK(entity0.GetId() == 0);
    CHECK(entity1.GetId() == 1);
    CHECK(entity2.GetId() == 2);

    CHECK(entity0.IsEnabled());
    CHECK(entity1.IsEnabled());
    CHECK_FALSE(entity2.IsEnabled());

    entity0.Disable();
    CHECK_FALSE(entity0.IsEnabled());

    entity0.Enable();
    CHECK(entity0.IsEnabled());
}

TEST_CASE("Entity: Component manipulations", "[Core/ECS]") {
    Fl::Entity entity(0);

    CHECK(entity.GetComponents().empty());
    CHECK_FALSE(entity.HasComponent<FirstTestComponent>());
    CHECK_FALSE(entity.HasComponent<SecondTestComponent>());

    entity.AddComponent<FirstTestComponent>();
    CHECK(entity.HasComponent<FirstTestComponent>());
    CHECK_NOTHROW(entity.GetComponent<FirstTestComponent>());
    CHECK_FALSE(entity.HasComponent<SecondTestComponent>());
    CHECK_THROWS(entity.GetComponent<SecondTestComponent>());

    CHECK(std::is_same_v<decltype(entity.GetComponent<FirstTestComponent>()), FirstTestComponent&>);

    entity.RemoveComponent<FirstTestComponent>();
    CHECK_FALSE(entity.HasComponent<FirstTestComponent>());
    CHECK_THROWS(entity.GetComponent<FirstTestComponent>());
}

TEST_CASE("Entity: Bitset", "[Core/ECS]") {
    Fl::Entity entity0(0);
    Fl::Entity entity1(1);

    CHECK(entity0.GetEnabledComponents().IsEmpty());
    entity0.AddComponent<FirstTestComponent>();
    CHECK_FALSE(entity0.GetEnabledComponents().IsEmpty());

    CHECK(entity0.GetEnabledComponents().GetEnabledBitCount() == 1);
    CHECK(entity0.GetEnabledComponents()[Fl::Component::GetId<FirstTestComponent>()]);
    CHECK(entity0.GetEnabledComponents().GetSize() == (Fl::Component::GetId<FirstTestComponent>() + 1));

    CHECK(entity1.GetEnabledComponents().IsEmpty());
    entity1.AddComponent<SecondTestComponent>();
    CHECK_FALSE(entity1.GetEnabledComponents().IsEmpty());

    CHECK(entity1.GetEnabledComponents().GetEnabledBitCount() == 1);
    CHECK(entity1.GetEnabledComponents()[Fl::Component::GetId<SecondTestComponent>()]);
    CHECK(entity1.GetEnabledComponents().GetSize() == (Fl::Component::GetId<SecondTestComponent>() + 1));

    //  Match test
    //     ---
    //    0 0 1     -> FirstTestComponent
    // &  0 1 0     -> SecondTestComponent
    //   _______
    // =  0 0 0
    CHECK((entity0.GetEnabledComponents() & entity1.GetEnabledComponents()) ==
        Fl::Bitset(entity1.GetEnabledComponents().GetSize(), false));
}
