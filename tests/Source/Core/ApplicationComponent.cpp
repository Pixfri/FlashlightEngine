// Copyright (C) 2025 Jean "Pixfri" Letessier
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.


#include <FlashlightEngine/Core/Application.hpp>
#include <FlashlightEngine/Core/ApplicationComponent.hpp>

#include <catch2/catch_test_macros.hpp>

class TestAppComponent final : public Fl::ApplicationComponent {
public:
	TestAppComponent(std::size_t maxRuns) : m_MaxRuns(maxRuns) {}
	~TestAppComponent() override = default;

	bool Update([[maybe_unused]] const Fl::FrameTimeInfo& timeInfo) override {
		UpdateCount++;

		return UpdateCount < m_MaxRuns;
	}

	std::size_t UpdateCount{ 0 };

private:
	std::size_t m_MaxRuns;
};

TEST_CASE("ApplicationComponent: Application component update", "[Core]") {
	Fl::Application testApp(0); // Do not reserve any world.

	// The max update number of the component is 3.
	auto& testAppComponent = testApp.AddComponent<TestAppComponent>(3);

	CHECK(testApp.GetAppComponents().size() == Fl::ApplicationComponent::GetId<TestAppComponent>() + 1);

	// This will update the application, and therefore the component only once.
	CHECK(testApp.RunOnce());
	CHECK(testAppComponent.UpdateCount == 1);

	CHECK(testApp.RunOnce());
	CHECK(testAppComponent.UpdateCount == 2);

	// RunOnce() returns false if there is no active components and no active world anymore.
	// Since there is no world in the first place, there is no active world, and the test components
	// has its max update count set to 3, so it stops when it reaches the 3rd update.
	CHECK_FALSE(testApp.RunOnce());
	CHECK(testAppComponent.UpdateCount == 3);

	// RunOnce() will still return false since there is no active component.
	// The component will not be updated since it was considered inactive at the last update.
	CHECK_FALSE(testApp.RunOnce());
	CHECK(testAppComponent.UpdateCount == 3);
}

TEST_CASE("ApplicationComponent: Application component removal", "[Core]") {
	Fl::Application testApp(0);

	auto& testAppComponent = testApp.AddComponent<TestAppComponent>(2);

	CHECK(testApp.RunOnce());
	CHECK(testAppComponent.UpdateCount == 1);

	CHECK(testApp.HasComponent<TestAppComponent>());

	testApp.RemoveComponent<TestAppComponent>();

	CHECK_FALSE(testApp.HasComponent<TestAppComponent>());

	// The component is removed, and there is no more components, so RunOnce() will return false.
	// Accessing the component isn't safe anymore since it has been released.
	CHECK_FALSE(testApp.RunOnce());
	CHECK(testApp.GetAppComponents().size() == Fl::ApplicationComponent::GetId<TestAppComponent>() + 1); // The memory is still reserved in case the component is added in the future.
}

TEST_CASE("ApplicationComponent: Retrieving a component", "[Core]") {
	Fl::Application testApp(0);

	// There is no world and no component, so RunOnce() will return false.
	// Retrieving a component that is not linked to the application will throw an exception.
	CHECK_FALSE(testApp.RunOnce());
	CHECK_THROWS(testApp.GetComponent<TestAppComponent>());

	testApp.AddComponent<TestAppComponent>(2);

	CHECK(testApp.RunOnce());
	CHECK_NOTHROW(testApp.GetComponent<TestAppComponent>());
}