// Copyright (C) 2025 Jean "Pixfri" Letessier
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#include <FlashlightEngine/Math/Angle.hpp>
#include <FlashlightEngine/Math/Matrix.hpp>
#include <FlashlightEngine/Math/Transform.hpp>

#include <catch2/catch_test_macros.hpp>

using namespace Fl::Literals;

TEST_CASE("Transform: Position", "[Math]") {
	Fl::Transform transform;
	CHECK(transform.GetPosition() == Fl::Vec3f(0.f, 0.f, 0.f));
	CHECK(transform.WasUpdated()); // By default, a transform always has an updated status
	transform.SetUpdated(false);

	transform.Translate(Fl::Vec3f(0.f, 0.f, 0.f));
	transform.SetPosition(0.f, 0.f, 0.f); // Equivalent of the above
	CHECK(transform.GetPosition() == Fl::Vec3f(0.f, 0.f, 0.f));
	CHECK(transform.WasUpdated());

	// Assigning a rotation for the following calls
	transform.SetRotation(90_deg, Fl::Axis::Y);
	transform.SetUpdated(false);

	// Move() performs a relative displacement, taking rotation into account
	transform.Move(Fl::Vec3f(0.f, 0.f, 1.f));
	transform.Move(0.f, 0.f, 1.f); // Equivalent of the above
	CHECK(transform.GetPosition() == Fl::Vec3f(2.f, 0.f, 0.f));
	CHECK(transform.WasUpdated());
	transform.SetUpdated(false);

	const Fl::Mat4f translationMat = transform.ComputeTranslationMatrix(false);
	CHECK(Fl::Mat3f(translationMat) == Fl::Mat3f::Identity());
	CHECK(Fl::Vec3f(translationMat.GetColumn(3)) == transform.GetPosition());
	CHECK_FALSE(transform.WasUpdated()); // Computing the translation matrix does not change the update state

	const Fl::Mat4f reversedTranslationMat = transform.ComputeTranslationMatrix(true); // Reversing the translation
	CHECK(Fl::Mat3f(reversedTranslationMat) == Fl::Mat3f::Identity());
	CHECK(Fl::Vec3f(reversedTranslationMat.GetColumn(3)) == -transform.GetPosition());
	CHECK_FALSE(transform.WasUpdated());
}

TEST_CASE("Transform: Rotation", "[Math]") {
	Fl::Transform transform;
	CHECK(transform.GetRotation() == Fl::Quaternionf::Identity());
	CHECK(transform.WasUpdated()); // By default, a transform always has an updated status
	transform.SetUpdated(false);

	transform.SetRotation(Fl::Quaternionf(90_deg, Fl::Axis::Y));
	transform.SetRotation(90_deg, Fl::Axis::Y); // Equivalent of the above
	CHECK(transform.GetRotation() == Fl::Quaternionf(90_deg, Fl::Axis::Y));
	CHECK(transform.WasUpdated()); // Changing the rotation does set the updated state
	transform.SetUpdated(false);

	transform.Rotate(Fl::Quaternionf(-45_deg, Fl::Axis::Y));
	transform.Rotate(-45_deg, Fl::Axis::Y); // Equivalent of the above
	CHECK(transform.GetRotation() == Fl::Quaternionf::Identity());
	CHECK(transform.WasUpdated());
	transform.SetUpdated(false);

	// Rotating on multiple axes at the same time; the rotation around X will be applied before the one around Y
	transform.Rotate(90_deg, 45_deg);
	CHECK(transform.GetRotation() == Fl::Quaternionf(45_deg, Fl::Axis::Y) * Fl::Quaternionf(90_deg, Fl::Axis::X));
	CHECK(transform.WasUpdated());
	transform.SetUpdated(false);
}

TEST_CASE("Transform: Scale", "[math]") {
	Fl::Transform transform;
	CHECK(transform.GetScale() == Fl::Vec3f(1.f, 1.f, 1.f));
	CHECK(transform.WasUpdated()); // By default, a transform always has an updated status
	transform.SetUpdated(false);

	transform.SetScale(2.f);
	transform.SetScale(Fl::Vec3f(2.f, 2.f, 2.f)); // Equivalent of the above
	transform.SetScale(2.f, 2.f, 2.f); // Equivalent of the above
	CHECK(transform.GetScale() == Fl::Vec3f(2.f, 2.f, 2.f));
	CHECK(transform.WasUpdated()); // Changing the scale does set the updated state
	transform.SetUpdated(false);

	// The scale is relative: 0.5 downscales the current value by half
	transform.Scale(0.5f);
	transform.Scale(Fl::Vec3f(0.5f, 0.5f, 0.5f)); // Equivalent of the above
	transform.Scale(0.5f, 0.5f, 0.5f); // Equivalent of the above
	CHECK(transform.GetScale() == Fl::Vec3f(0.25f, 0.25f, 0.25f));
	CHECK(transform.WasUpdated());
}

TEST_CASE("Transform: Operations", "[Math]") {
	Fl::Transform transform(Fl::Vec3f(1.f, 0.f, 1.f), Fl::Quaternionf(90_deg, Fl::Axis::Y), Fl::Vec3f(2.f));

	CHECK(transform.GetPosition() == Fl::Vec3f(1.f, 0.f, 1.f));
	CHECK(transform.GetRotation() == Fl::Quaternionf(90_deg, Fl::Axis::Y));
	CHECK(transform.GetScale() == Fl::Vec3f(2.f, 2.f, 2.f));

	transform.SetUpdated(false);
	Fl::Mat4f transformMat = transform.ComputeTransformMatrix();
	CHECK(Fl::Vec3f(transformMat.GetColumn(3)) == transform.GetPosition());
	CHECK(transformMat == Fl::Mat4f(0.f, 0.f, 2.f, 1.f,
		                            0.f, 2.f, 0.f, 0.f,
		                           -2.f, 0.f, 0.f, 1.f,
		                            0.f, 0.f, 0.f, 1.f));
	CHECK_FALSE(transform.WasUpdated()); // Computing the transform matrix doesn't change the update state

	transform.Rotate(45_deg, Fl::Axis::X);
	transform.Move(0.f, 0.f, -1.f);
	transform.SetScale(1.f);

	transform.SetUpdated(false);
	transformMat = transform.ComputeTransformMatrix();
	CHECK(Fl::Vec3f(transformMat.GetColumn(3)) == transform.GetPosition());
	CHECK(transformMat == Fl::Mat4f(-8.9407e-08f, 0.707106829f,  0.707106709f, 0.292893231f,
		                             0.f,         0.707106769f, -0.707106829f, 0.707106769f,
		                            -1.f,         0.f,          -1.19209e-07f, 1.f,
		                             0.f,         0.f,           0.f,          1.f));
	CHECK_FALSE(transform.WasUpdated());
}