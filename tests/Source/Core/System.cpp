// Copyright (C) 2025 Jean "Pixfri" Letessier
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.


#include <FlashlightEngine/Core/Application.hpp>
#include <FlashlightEngine/Core/Component.hpp>
#include <FlashlightEngine/Core/Entity.hpp>
#include <FlashlightEngine/Core/System.hpp>
#include <FlashlightEngine/Core/World.hpp>

#include <catch2/catch_test_macros.hpp>

namespace {
    struct FirstTestComponent final : public Fl::Component {
    };

    struct SecondTestComponent final : public Fl::Component {
    };
}

class TestSystem final : public Fl::System {
public:
    TestSystem() {
        RegisterComponents<SecondTestComponent>();
    }

    void LinkEntity(const Fl::EntityPtr& entity) override {
        ++LinkedEntityCount;
        System::LinkEntity(entity);
    }

    void UnlinkEntity(const Fl::EntityPtr& entity) override {
        --LinkedEntityCount;
        System::UnlinkEntity(entity);
    }

    bool Update([[maybe_unused]] const Fl::FrameTimeInfo& timeInfo) override {
        ++UpdateCount;
        return UpdateCount != 4;
    }

    Fl::U64 LinkedEntityCount = 0;
    Fl::U64 UpdateCount = 0;
};

TEST_CASE("System: System update", "[Core/ECS]") {
    Fl::World world(3);

    auto& testSystem = world.AddSystem<TestSystem>();
    CHECK(testSystem.GetAcceptedComponents().GetEnabledBitCount() == 1);
    // A system's accepted components bitset gets a value at the component's index, thus is resized accordingly
    CHECK(testSystem.GetAcceptedComponents().GetSize() == Fl::Component::GetId<SecondTestComponent>() + 1);
    CHECK(testSystem.GetAcceptedComponents()[Fl::Component::GetId<SecondTestComponent>()]);

    Fl::Entity& entity0 = world.AddEntity();
    entity0.AddComponent<FirstTestComponent>();
    
    // As the entity doesn't contain any of the accepted components, it won't be linked
    // The world's update returns true while there are still active systems
    CHECK(world.Update({}));
    CHECK_FALSE(testSystem.ContainsEntity(entity0));
    CHECK(testSystem.LinkedEntityCount == 0);
    CHECK(testSystem.UpdateCount == 1);

    Fl::Entity& entity1 = world.AddEntity();
    entity1.AddComponent<SecondTestComponent>();

    CHECK(world.Update({})); // The entity is linked to the system as it has at least one of the accepted components
    CHECK(testSystem.ContainsEntity(entity1));
    CHECK(testSystem.LinkedEntityCount == 1);
    CHECK(testSystem.UpdateCount == 2);

    // Removing the component, the entity shouldn't be processed by the system anymore
    entity1.RemoveComponent<SecondTestComponent>();

    CHECK(world.Update({})); // The entity is unlinked from the system
    CHECK_FALSE(testSystem.ContainsEntity(entity1));
    CHECK(testSystem.LinkedEntityCount == 0);
    CHECK(testSystem.UpdateCount == 3);
    
    // Creating an entity without components
    const Fl::Entity& emptyEntity = world.AddEntity();

    // The entity will not be linked since there's no component to be matched
    // As the only existing system has returned false on update there are no active system anymore, thus the world update also returns false
    CHECK_FALSE(world.Update({}));
    CHECK_FALSE(testSystem.ContainsEntity(emptyEntity));
    CHECK(testSystem.LinkedEntityCount == 0);
    CHECK(testSystem.UpdateCount == 4);
    
    // The latest system update made it return false; this system should not be updated ever again and should ignore all entities, even compatible ones
    entity1.AddComponent<SecondTestComponent>();
    CHECK_FALSE(world.Update({}));
    CHECK_FALSE(testSystem.ContainsEntity(entity1));
    CHECK(testSystem.LinkedEntityCount == 0);
    CHECK(testSystem.UpdateCount == 4);
}
