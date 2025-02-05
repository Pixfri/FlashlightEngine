// Copyright (C) 2025 Jean "Pixfri" Letessier
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#include <FlashlightEngine/Core/World.hpp>

#include <catch2/catch_test_macros.hpp>

TEST_CASE("World: Entity manipulations", "[Core/ECS]") {
	Fl::World world(3);

	// The constructor argument simply reserves the memory for entities
	CHECK(world.GetEntities().empty());

	const Fl::Entity& entity0 = world.AddEntity();
	const Fl::Entity& entity1 = world.AddEntity();
	const Fl::Entity& entity2 = world.AddEntity();

	CHECK(entity0.GetId() == 0);
	CHECK(entity1.GetId() == 1);
	CHECK(entity2.GetId() == 2);

	CHECK(world.GetEntities().size() == 3);
	CHECK(world.GetEntities()[0]->GetId() == 0);
	CHECK(world.GetEntities()[1]->GetId() == 1);
	CHECK(world.GetEntities()[2]->GetId() == 2);

	world.RemoveEntity(entity0);
	CHECK(world.GetEntities().size() == 2);
	CHECK(world.GetEntities()[0]->GetId() == 1);
	CHECK(world.GetEntities()[1]->GetId() == 2);

	world.RemoveEntity(entity2);
	CHECK(world.GetEntities().size() == 1);
	CHECK(world.GetEntities()[0]->GetId() == 1);

	world.AddEntity();
	CHECK(world.GetEntities().size() == 2);
	CHECK(world.GetEntities()[0]->GetId() == 1);
	CHECK(world.GetEntities()[1]->GetId() == 3);

	// The entity removal is made by checking the pointers; if it isn't owned by this world, it throws an exception
	const Fl::Entity extEntity(0);
	CHECK_THROWS(world.RemoveEntity(extEntity));
}

TEST_CASE("World: Get entities with components", "[Core/ECS]") {
	struct TestComponent1 final : public Fl::Component {};
	struct TestComponent2 final : public Fl::Component {};

	Fl::World world(2);

	Fl::Entity& entity1 = world.AddEntity();
	Fl::Entity& entity2 = world.AddEntity();

	CHECK(world.GetEntitiesWithComponents<TestComponent1>().empty());
	CHECK(world.GetEntitiesWithComponents<TestComponent2>().empty());
	CHECK(world.GetEntitiesWithComponents<TestComponent1, TestComponent2>().empty());

	entity1.AddComponent<TestComponent1>();
	entity2.AddComponent<TestComponent2>();
	REQUIRE(world.GetEntitiesWithComponents<TestComponent1>().size() == 1);
	CHECK(world.GetEntitiesWithComponents<TestComponent1>().front() == &entity1);
	REQUIRE(world.GetEntitiesWithComponents<TestComponent2>().size() == 1);
	CHECK(world.GetEntitiesWithComponents<TestComponent2>().front() == &entity2);
	CHECK(world.GetEntitiesWithComponents<TestComponent1, TestComponent2>().empty()); // No entity has both components

	entity1.AddComponent<TestComponent2>();
	CHECK(world.GetEntitiesWithComponents<TestComponent1>().front() == &entity1); // Fetching with the first component is still valid
	REQUIRE(world.GetEntitiesWithComponents<TestComponent1, TestComponent2>().size() == 1);
	CHECK(world.GetEntitiesWithComponents<TestComponent1, TestComponent2>().front() == &entity1);
	CHECK(world.GetEntitiesWithComponents<TestComponent2, TestComponent1>().front() == &entity1); // order doesn't matter

	for (Fl::Entity* entity : world.GetEntitiesWithComponents<TestComponent1, TestComponent2>()) {
		CHECK(entity->HasComponent<TestComponent1>());
		CHECK(entity->HasComponent<TestComponent2>());
	}

	entity1.RemoveComponent<TestComponent2>();
	CHECK(world.GetEntitiesWithComponents<TestComponent1, TestComponent2>().empty());
	CHECK(world.GetEntitiesWithComponents<TestComponent1>().front() == &entity1);
}

TEST_CASE("World: Refreshing", "[Core/ECS]") {
	Fl::World world(3);
	
	Fl::Entity& entity0 = world.AddEntity(true);
	Fl::Entity& entity1 = world.AddEntity(true);
	const Fl::Entity& entity2 = world.AddEntity(true);

	CHECK(entity0.GetId() == 0);
	CHECK(entity1.GetId() == 1);
	CHECK(entity2.GetId() == 2);

	world.Refresh(); // Nothing to reorganize, should not change the result

	CHECK(world.GetEntities()[0]->GetId() == 0);
	CHECK(world.GetEntities()[1]->GetId() == 1);
	CHECK(world.GetEntities()[2]->GetId() == 2);

	// Disabling the entity 1; refreshing the world should put it at the end of the list

	entity1.Disable();
	world.Refresh();

	// [ 0; 1;  2 ]
	//   |    X
	// [ 0; 2; 1d ]

	CHECK(world.GetEntities()[0]->GetId() == 0);
	CHECK(world.GetEntities()[1]->GetId() == 2);
	CHECK(world.GetEntities()[2]->GetId() == 1);

	// Likewise for the entity 0, it ends swapped with the last enabled entity

	entity0.Disable();
	world.Refresh();

	// [ 0;  2; 1d ]
	//     X    |
	// [ 2; 0d; 1d ]

	CHECK(world.GetEntities()[0]->GetId() == 2);
	CHECK(world.GetEntities()[1]->GetId() == 0);
	CHECK(world.GetEntities()[2]->GetId() == 1);

	// Re-enabling the entity 1 should make it advance so that the 0 remains at the end

	entity1.Enable();
	world.Refresh();

	// [ 2; 0d; 1d ]
	//   |     X
	// [ 2;  1; 0d ]

	CHECK(world.GetEntities()[0]->GetId() == 2);
	CHECK(world.GetEntities()[1]->GetId() == 1);
	CHECK(world.GetEntities()[2]->GetId() == 0);
}