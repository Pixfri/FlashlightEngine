// Copyright (C) 2025 Jean "Pixfri" Letessier
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#include <FlashlightEngine/Math/Utils.hpp>
#include <FlashlightEngine/Math/Vector.hpp>

#include <FlTests/CustomMatchers.hpp>

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Utils: Lerp arithmetic types", "[Math]") {
	CHECK(Fl::MathUtils::Lerp(0.f, 1.f, 0.f) == 0.f);
	CHECK(Fl::MathUtils::Lerp(0.f, 1.f, 0.25f) == 0.25f);
	CHECK(Fl::MathUtils::Lerp(0.f, 1.f, 0.5f) == 0.5f);
	CHECK(Fl::MathUtils::Lerp(0.f, 1.f, 0.75f) == 0.75f);
	CHECK(Fl::MathUtils::Lerp(0.f, 1.f, 1.f) == 1.f);

	CHECK(Fl::MathUtils::Lerp(-5.f, 5.f, 0.f) == -5.f);
	CHECK(Fl::MathUtils::Lerp(-5.f, 5.f, 0.25f) == -2.5f);
	CHECK(Fl::MathUtils::Lerp(-5.f, 5.f, 0.5f) == 0.f);
	CHECK(Fl::MathUtils::Lerp(-5.f, 5.f, 0.75f) == 2.5f);
	CHECK(Fl::MathUtils::Lerp(-5.f, 5.f, 1.f) == 5.f);

	CHECK(Fl::MathUtils::Lerp(-1.43f, 8.12f, 0.f) == -1.43f);
	CHECK(Fl::MathUtils::Lerp(-1.43f, 8.12f, 0.25f) == 0.9575f);
	CHECK(Fl::MathUtils::Lerp(-1.43f, 8.12f, 0.5f) == 3.345f);
	CHECK(Fl::MathUtils::Lerp(-1.43f, 8.12f, 0.75f) == 5.7325001f);
	CHECK(Fl::MathUtils::Lerp(-1.43f, 8.12f, 1.f) == 8.12f);

	CHECK(Fl::MathUtils::Lerp(457, 5324, 0.f) == 457);
	CHECK(Fl::MathUtils::Lerp(457, 5324, 0.25f) == 1673);
	CHECK(Fl::MathUtils::Lerp(457, 5324, 0.5f) == 2890);
	CHECK(Fl::MathUtils::Lerp(457, 5324, 0.75f) == 4107);
	CHECK(Fl::MathUtils::Lerp(457, 5324, 1.f) == 5324);
}

TEST_CASE("MathUtils: Lerp vector", "[Math]") {
	CHECK(Fl::MathUtils::Lerp(Fl::Vec3f(0.f), Fl::Vec3f(1.f), 0.f) == Fl::Vec3f(0.f));
	CHECK(Fl::MathUtils::Lerp(Fl::Vec3f(0.f), Fl::Vec3f(1.f), 0.25f) == Fl::Vec3f(0.25f));
	CHECK(Fl::MathUtils::Lerp(Fl::Vec3f(0.f), Fl::Vec3f(1.f), 0.5f) == Fl::Vec3f(0.5f));
	CHECK(Fl::MathUtils::Lerp(Fl::Vec3f(0.f), Fl::Vec3f(1.f), 0.75f) == Fl::Vec3f(0.75f));
	CHECK(Fl::MathUtils::Lerp(Fl::Vec3f(0.f), Fl::Vec3f(1.f), 1.f) == Fl::Vec3f(1.f));
						 
	CHECK(Fl::MathUtils::Lerp(Fl::Vec3f(1.f, 2.f, 3.f), Fl::Vec3f(3.f, 2.f, 1.f), 0.f) == Fl::Vec3f(1.f, 2.f, 3.f));
	CHECK(Fl::MathUtils::Lerp(Fl::Vec3f(1.f, 2.f, 3.f), Fl::Vec3f(3.f, 2.f, 1.f), 0.25f) == Fl::Vec3f(1.5f, 2.f, 2.5f));
	CHECK(Fl::MathUtils::Lerp(Fl::Vec3f(1.f, 2.f, 3.f), Fl::Vec3f(3.f, 2.f, 1.f), 0.5f) == Fl::Vec3f(2.f, 2.f, 2.f));
	CHECK(Fl::MathUtils::Lerp(Fl::Vec3f(1.f, 2.f, 3.f), Fl::Vec3f(3.f, 2.f, 1.f), 0.75f) == Fl::Vec3f(2.5f, 2.f, 1.5f));
	CHECK(Fl::MathUtils::Lerp(Fl::Vec3f(1.f, 2.f, 3.f), Fl::Vec3f(3.f, 2.f, 1.f), 1.f) == Fl::Vec3f(3.f, 2.f, 1.f));
						 
	CHECK(Fl::MathUtils::Lerp(Fl::Vec3i(140, 874, 3541), Fl::Vec3i(841, 53, 4712), 0.f) == Fl::Vec3i(140, 874, 3541));
	CHECK(Fl::MathUtils::Lerp(Fl::Vec3i(140, 874, 3541), Fl::Vec3i(841, 53, 4712), 0.25f) == Fl::Vec3i(315, 668, 3833));
	CHECK(Fl::MathUtils::Lerp(Fl::Vec3i(140, 874, 3541), Fl::Vec3i(841, 53, 4712), 0.5f) == Fl::Vec3i(490, 463, 4126));
	CHECK(Fl::MathUtils::Lerp(Fl::Vec3i(140, 874, 3541), Fl::Vec3i(841, 53, 4712), 0.75f) == Fl::Vec3i(665, 258, 4419));
	CHECK(Fl::MathUtils::Lerp(Fl::Vec3i(140, 874, 3541), Fl::Vec3i(841, 53, 4712), 1.f) == Fl::Vec3i(841, 53, 4712));
						 
	CHECK(Fl::MathUtils::Lerp(Fl::Vec3b(255, 140, 27), Fl::Vec3b(75, 241, 173), 0.f) == Fl::Vec3b(255, 140, 27));
	CHECK(Fl::MathUtils::Lerp(Fl::Vec3b(255, 140, 27), Fl::Vec3b(75, 241, 173), 0.25f) == Fl::Vec3b(210, 165, 63));
	CHECK(Fl::MathUtils::Lerp(Fl::Vec3b(255, 140, 27), Fl::Vec3b(75, 241, 173), 0.5f) == Fl::Vec3b(165, 190, 100));
	CHECK(Fl::MathUtils::Lerp(Fl::Vec3b(255, 140, 27), Fl::Vec3b(75, 241, 173), 0.75f) == Fl::Vec3b(120, 215, 136));
	CHECK(Fl::MathUtils::Lerp(Fl::Vec3b(255, 140, 27), Fl::Vec3b(75, 241, 173), 1.f) == Fl::Vec3b(75, 241, 173));
}

TEST_CASE("MathUtils: Smoothstep value", "[Math]") {
	CHECK(Fl::MathUtils::Smoothstep(0.f) == 0.f);
	CHECK(Fl::MathUtils::Smoothstep(0.25f) == 0.15625f);
	CHECK(Fl::MathUtils::Smoothstep(0.5f) == 0.5f);
	CHECK(Fl::MathUtils::Smoothstep(0.75f) == 0.84375f);
	CHECK(Fl::MathUtils::Smoothstep(1.f) == 1.f);
}

TEST_CASE("MathUtils: Smoothstep range", "[Math]") {
	CHECK(Fl::MathUtils::Smoothstep(0.f, 1.f, -1.f) == 0.f); // If value < minThresh, returns 0
	CHECK(Fl::MathUtils::Smoothstep(0.f, 1.f, 0.f) == 0.f);
	CHECK(Fl::MathUtils::Smoothstep(0.f, 1.f, 0.25f) == 0.15625f);
	CHECK(Fl::MathUtils::Smoothstep(0.f, 1.f, 0.5f) == 0.5f);
	CHECK(Fl::MathUtils::Smoothstep(0.f, 1.f, 0.75f) == 0.84375f);
	CHECK(Fl::MathUtils::Smoothstep(0.f, 1.f, 1.f) == 1.f);
	CHECK(Fl::MathUtils::Smoothstep(0.f, 1.f, 2.f) == 1.f); // If value > maxThresh, returns 1

	CHECK(Fl::MathUtils::Smoothstep(-5.f, 5.f, -10.f) == 0.f);
	CHECK(Fl::MathUtils::Smoothstep(-5.f, 5.f, -5.f - std::numeric_limits<float>::epsilon()) == 0.f);
	CHECK(Fl::MathUtils::Smoothstep(-5.f, 5.f, -5.f) == 0.f);
	CHECK(Fl::MathUtils::Smoothstep(-5.f, 5.f, -4.5f) == 0.00725000072f);
	CHECK(Fl::MathUtils::Smoothstep(-5.f, 5.f, -2.5f) == 0.15625f);
	CHECK(Fl::MathUtils::Smoothstep(-5.f, 5.f, -0.5f) == 0.42524996f);
	CHECK(Fl::MathUtils::Smoothstep(-5.f, 5.f, 0.f) == 0.5f);
	CHECK(Fl::MathUtils::Smoothstep(-5.f, 5.f, 0.5f) == 0.57475001f);
	CHECK(Fl::MathUtils::Smoothstep(-5.f, 5.f, 2.5f) == 0.84375f);
	CHECK(Fl::MathUtils::Smoothstep(-5.f, 5.f, 4.5f) == 0.99274999f);
	CHECK(Fl::MathUtils::Smoothstep(-5.f, 5.f, 5.f) == 1.f);
	CHECK(Fl::MathUtils::Smoothstep(-5.f, 5.f, 5.f + std::numeric_limits<float>::epsilon()) == 1.f);
	CHECK(Fl::MathUtils::Smoothstep(-5.f, 5.f, 10.f) == 1.f);
}

TEST_CASE("MathUtils: Smootherstep value", "[Math]") {
	CHECK(Fl::MathUtils::Smootherstep(0.f) == 0.f);
	CHECK(Fl::MathUtils::Smootherstep(0.25f) == 0.103515625f);
	CHECK(Fl::MathUtils::Smootherstep(0.5f) == 0.5f);
	CHECK(Fl::MathUtils::Smootherstep(0.75f) == 0.89648438f);
	CHECK(Fl::MathUtils::Smootherstep(1.f) == 1.f);
}

TEST_CASE("MathUtils: Smootherstep range", "[Math]") {
	CHECK(Fl::MathUtils::Smootherstep(0.f, 1.f, -1.f) == 0.f); // If value < minThresh, returns 0
	CHECK(Fl::MathUtils::Smootherstep(0.f, 1.f, 0.f) == 0.f);
	CHECK(Fl::MathUtils::Smootherstep(0.f, 1.f, 0.25f) == 0.103515625f);
	CHECK(Fl::MathUtils::Smootherstep(0.f, 1.f, 0.5f) == 0.5f);
	CHECK(Fl::MathUtils::Smootherstep(0.f, 1.f, 0.75f) == 0.89648438f);
	CHECK(Fl::MathUtils::Smootherstep(0.f, 1.f, 1.f) == 1.f);
	CHECK(Fl::MathUtils::Smootherstep(0.f, 1.f, 2.f) == 1.f); // If value > maxThresh, returns 1

	CHECK(Fl::MathUtils::Smootherstep(-5.f, 5.f, -10.f) == 0.f);
	CHECK(Fl::MathUtils::Smootherstep(-5.f, 5.f, -5.f - std::numeric_limits<float>::epsilon()) == 0.f);
	CHECK(Fl::MathUtils::Smootherstep(-5.f, 5.f, -5.f) == 0.f);
	CHECK(Fl::MathUtils::Smootherstep(-5.f, 5.f, -4.5f) == 0.0011581251f);
	CHECK(Fl::MathUtils::Smootherstep(-5.f, 5.f, -2.5f) == 0.103515625f);
	CHECK(Fl::MathUtils::Smootherstep(-5.f, 5.f, -0.5f) == 0.40687308f);
	CHECK(Fl::MathUtils::Smootherstep(-5.f, 5.f, 0.f) == 0.5f);
	CHECK(Fl::MathUtils::Smootherstep(-5.f, 5.f, 0.5f) == 0.59312695f);
	CHECK(Fl::MathUtils::Smootherstep(-5.f, 5.f, 2.5f) == 0.89648438f);
	CHECK(Fl::MathUtils::Smootherstep(-5.f, 5.f, 4.5f) == 0.99884182f);
	CHECK(Fl::MathUtils::Smootherstep(-5.f, 5.f, 5.f) == 1.f);
	CHECK(Fl::MathUtils::Smootherstep(-5.f, 5.f, 5.f + std::numeric_limits<float>::epsilon()) == 1.f);
	CHECK(Fl::MathUtils::Smootherstep(-5.f, 5.f, 10.f) == 1.f);
}

TEST_CASE("MathUtils: Orthonormal basis", "[Math]") {
	Fl::Vec3f axis2;
	Fl::Vec3f axis3;

	Fl::MathUtils::ComputeOrthonormalBasis(Fl::Axis::X, axis2, axis3);
	CHECK(axis2 == -Fl::Axis::Z);
	CHECK(axis3 == Fl::Axis::Y);

	Fl::MathUtils::ComputeOrthonormalBasis(Fl::Axis::Y, axis2, axis3);
	CHECK(axis2 == Fl::Axis::X);
	CHECK(axis3 == -Fl::Axis::Z);

	Fl::MathUtils::ComputeOrthonormalBasis(Fl::Axis::Z, axis2, axis3);
	CHECK(axis2 == Fl::Axis::X);
	CHECK(axis3 == Fl::Axis::Y);

	Fl::MathUtils::ComputeOrthonormalBasis(Fl::Vec3f(1.f).Normalize(), axis2, axis3);
	CHECK(axis2 == Fl::Vec3f(0.788675f, -0.211324856f, -0.577350259f));
	CHECK(axis3 == Fl::Vec3f(-0.211324856f, 0.788675f, -0.577350259f));
	CHECK(axis2.Dot(axis3) == 0.f);

	Fl::MathUtils::ComputeOrthonormalBasis(Fl::Vec3f(-1.f).Normalize(), axis2, axis3);
	CHECK(axis2 == Fl::Vec3f(0.788675f, -0.211324856f, -0.577350259f));
	CHECK(axis3 == Fl::Vec3f(0.211324856f, -0.788675f, 0.577350259f));
	CHECK(axis2.Dot(axis3) == 0.f);
}

TEST_CASE("MathUtils: Fibonacci sphere", "[Math]") {
	std::vector<Fl::Vec3f> fiboPoints = Fl::MathUtils::ComputeFibonacciSpherePoints(0);
	CHECK(fiboPoints.empty());

	fiboPoints = Fl::MathUtils::ComputeFibonacciSpherePoints(1);
	REQUIRE(fiboPoints.size() == 1);
	CHECK_THAT(fiboPoints[0], IsNearlyEqualToVector(Fl::Vec3f(1.f, 0.f, 0.f)));

	fiboPoints = Fl::MathUtils::ComputeFibonacciSpherePoints(2);
	REQUIRE(fiboPoints.size() == 2);
	CHECK_THAT(fiboPoints[0], IsNearlyEqualToVector(Fl::Vec3f(0.8660253f, 0.5f, 0.f)));
	CHECK_THAT(fiboPoints[1], IsNearlyEqualToVector(Fl::Vec3f(-0.6385802f, -0.5, -0.5849917f)));

	fiboPoints = Fl::MathUtils::ComputeFibonacciSpherePoints(3);
	REQUIRE(fiboPoints.size() == 3);
	CHECK_THAT(fiboPoints[0], IsNearlyEqualToVector(Fl::Vec3f(0.745356f, 0.6666666f, 0.f)));
	CHECK_THAT(fiboPoints[1], IsNearlyEqualToVector(Fl::Vec3f(-0.7373688f, 0.f, -0.6754903f)));
	CHECK_THAT(fiboPoints[2], IsNearlyEqualToVector(Fl::Vec3f(0.0651632f, -0.6666666f, 0.742502f)));

	fiboPoints = Fl::MathUtils::ComputeFibonacciSpherePoints(4);
	REQUIRE(fiboPoints.size() == 4);
	CHECK_THAT(fiboPoints[0], IsNearlyEqualToVector(Fl::Vec3f(0.6614378f, 0.75f, 0.f)));
	CHECK_THAT(fiboPoints[1], IsNearlyEqualToVector(Fl::Vec3f(-0.7139543f, 0.25f, -0.6540406f)));
	CHECK_THAT(fiboPoints[2], IsNearlyEqualToVector(Fl::Vec3f(0.0846495f, -0.25f, 0.9645385f)));
	CHECK_THAT(fiboPoints[3], IsNearlyEqualToVector(Fl::Vec3f(0.4024442f, -0.75f, -0.5249176f)));

	fiboPoints = Fl::MathUtils::ComputeFibonacciSpherePoints(5);
	REQUIRE(fiboPoints.size() == 5);
	CHECK_THAT(fiboPoints[0], IsNearlyEqualToVector(Fl::Vec3f(0.6f, 0.8f, 0.f)));
	CHECK_THAT(fiboPoints[1], IsNearlyEqualToVector(Fl::Vec3f(-0.6758097f, 0.3999999f, -0.619097f)));
	CHECK_THAT(fiboPoints[2], IsNearlyEqualToVector(Fl::Vec3f(0.0874256f, 0.f, 0.996171f)));
	CHECK_THAT(fiboPoints[3], IsNearlyEqualToVector(Fl::Vec3f(0.5576431f, -0.3999999f, -0.7273473f)));
	CHECK_THAT(fiboPoints[4], IsNearlyEqualToVector(Fl::Vec3f(-0.5908281f, -0.7999999f, 0.104509f)));
}