// Copyright (C) 2025 Jean "Pixfri" Letessier
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#include <FlashlightEngine/Math/Angle.hpp>
#include <FlashlightEngine/Math/Matrix.hpp>
#include <FlashlightEngine/Math/Quaternion.hpp>

#include <FlTests/CustomMatchers.hpp>

#include <catch2/catch_test_macros.hpp>

using namespace Fl::Literals;

namespace {

	const Fl::Quaternionf quat1(10_deg, Fl::Axis::X);
	const Fl::Quaternionf quat2(90_deg, 1.f, -2.f, 5.f);
	const Fl::Quaternionf quat3(180_deg, 1.f, -2.f, 5.f);

} // namespace

TEST_CASE("Quaternion: Deduction guides", "[Math]") {
	const Fl::Quaternion quatfVal(1.f, 1.f, 1.f, 1.f);
	const Fl::Quaternion quatfDeg(Fl::Degrees(0.f), Fl::Axis::X);
	const Fl::Quaternion quatdRad(Fl::Radians(0.0), 1.0, 0.0, 0.0);

	CHECK(std::is_same_v<std::decay_t<decltype(quatfVal)>, Fl::Quaternionf>);
	CHECK(std::is_same_v<std::decay_t<decltype(quatfDeg)>, Fl::Quaternionf>);
	CHECK(std::is_same_v<std::decay_t<decltype(quatdRad)>, Fl::Quaterniond>);
}

TEST_CASE("Quaternion: Dot product", "[Math]") {
	const Fl::Quaternionf normedQuat1 = quat1.Normalize();
	const Fl::Quaternionf normedQuat2 = quat2.Normalize();
	const Fl::Quaternionf normedQuat3 = quat3.Normalize();

	CHECK(quat1.Dot(quat1) == 1.f);
	CHECK(normedQuat1.Dot(normedQuat1) == 1.f);

	CHECK(quat2.Dot(quat2) == 15.5f);
	CHECK_THAT(normedQuat2.Dot(normedQuat2), IsNearlyEqualTo(1.f));

	CHECK(quat3.Dot(quat3) == 30.f);
	CHECK_THAT(normedQuat3.Dot(normedQuat3), IsNearlyEqualTo(1.f));

	CHECK_THAT(quat1.Dot(quat2), IsNearlyEqualTo(0.7660444f));
	CHECK_THAT(normedQuat1.Dot(normedQuat2), IsNearlyEqualTo(0.1945755f));
	CHECK(quat1.Dot(quat2) == quat2.Dot(quat1));

	CHECK_THAT(quat3.Dot(quat1), IsNearlyEqualTo(0.0871557f));
	CHECK_THAT(normedQuat3.Dot(normedQuat1), IsNearlyEqualTo(0.0159124f));
	CHECK(quat3.Dot(quat1) == quat1.Dot(quat3));
}

TEST_CASE("Quaternion: Norm computation", "[Math]") {
	CHECK_THAT(quat1.ComputeSquaredNorm(), IsNearlyEqualTo(1.f));
	CHECK_THAT(quat1.ComputeNorm(), IsNearlyEqualTo(1.f));

	CHECK_THAT(quat2.ComputeSquaredNorm(), IsNearlyEqualTo(15.5f));
	CHECK_THAT(quat2.ComputeNorm(), IsNearlyEqualTo(3.93700385f));

	CHECK_THAT(quat3.ComputeSquaredNorm(), IsNearlyEqualTo(30.f));
	CHECK_THAT(quat3.ComputeNorm(), IsNearlyEqualTo(5.47722578f));
}

TEST_CASE("Quaternion: Interpolation", "[math]") {
	// Results taken from Rust's nalgebra crate: https://play.rust-lang.org/?version=stable&mode=release&edition=2018&gist=fb93e96b13f3708b3b54aacef65365c2

	CHECK(quat1.Lerp(quat2, 0.f) == quat1);
	CHECK_THAT(quat1.Lerp(quat2, 0.25f), IsNearlyEqualToQuaternion(Fl::Quaternionf(0.92392272f, 0.2421435f, -0.35355338f, 0.88388348f)));
	CHECK_THAT(quat1.Lerp(quat2, 0.5f), IsNearlyEqualToQuaternion(Fl::Quaternionf(0.85165071f, 0.39713126f, -0.70710677f, 1.767767f)));
	CHECK_THAT(quat1.Lerp(quat2, 0.75f), IsNearlyEqualToQuaternion(Fl::Quaternionf(0.77937877f, 0.552119f, -1.06066f, 2.6516504f)));
	CHECK(quat1.Lerp(quat2, 1.f) == quat2);

	const Fl::Quaternionf normedQuat1 = quat1.Normalize();
	const Fl::Quaternionf normedQuat2 = quat2.Normalize();

	// Direct NLerp() (quaternions are not normalized beforehand)
	CHECK(quat1.NLerp(quat2, 0.f) == normedQuat1);
	CHECK_THAT(quat1.NLerp(quat2, 0.25f), IsNearlyEqualToQuaternion(Fl::Quaternionf(0.68513638f, 0.179562f, -0.262178f, 0.65544516f)));
	CHECK_THAT(quat1.NLerp(quat2, 0.5f), IsNearlyEqualToQuaternion(Fl::Quaternionf(0.40111461f, 0.18704282f, -0.3330366f, 0.83259147f)));
	CHECK_THAT(quat1.NLerp(quat2, 0.75f), IsNearlyEqualToQuaternion(Fl::Quaternionf(0.25880963f, 0.183343f, -0.3522152f, 0.880538f)));
	CHECK(quat1.NLerp(quat2, 1.f) == normedQuat2);

	// Pre-normalized NLerp() (almost strictly equivalent to NLerp(...).normalize() with non-normalized quaternions like above)
	CHECK(normedQuat1.NLerp(normedQuat2, 0.f) == normedQuat1);
	CHECK_THAT(normedQuat1.NLerp(normedQuat2, 0.25f), IsNearlyEqualToQuaternion(Fl::Quaternionf(0.94805622f, 0.13198756f, -0.10749099f, 0.26872748f)));
	CHECK_THAT(normedQuat1.NLerp(normedQuat2, 0.5f), IsNearlyEqualToQuaternion(Fl::Quaternionf(0.76069689f, 0.17258403f, -0.23239529f, 0.58098823f)));
	CHECK_THAT(normedQuat1.NLerp(normedQuat2, 0.75f), IsNearlyEqualToQuaternion(Fl::Quaternionf(0.45934007f, 0.18731718f, -0.32247299f, 0.8061825f)));
	CHECK(normedQuat1.NLerp(normedQuat2, 1.f) == normedQuat2);

	CHECK(normedQuat1.SLerp(normedQuat2, 0.f) == normedQuat1);
	CHECK_THAT(normedQuat1.SLerp(normedQuat2, 0.25f), IsNearlyEqualToQuaternion(Fl::Quaternionf(0.93302816f, 0.13793936f, -0.12341759f, 0.30854398f)));
	CHECK_THAT(normedQuat1.SLerp(normedQuat2, 0.5f), IsNearlyEqualToQuaternion(Fl::Quaternionf(0.76069695f, 0.17258403f, -0.23239529f, 0.58098823f)));
	CHECK_THAT(normedQuat1.SLerp(normedQuat2, 0.75f), IsNearlyEqualToQuaternion(Fl::Quaternionf(0.49936396f, 0.18703631f, -0.31418267f, 0.78545672f)));
	CHECK(normedQuat1.SLerp(normedQuat2, 1.f) == normedQuat2);
}

TEST_CASE("Quaternion: Matrix computation", "[Math]") {
	const Fl::Mat4f quat1Mat = quat1.ComputeMatrix();
	const Fl::Mat4f quat2Mat = quat2.ComputeMatrix();
	const Fl::Mat4f quat3Mat = quat3.ComputeMatrix();

	// Checking that quaternion multiplication is symmetrical to the matrices'
	// The matrix multiplication may introduce a slight difference compared to the quaternion's
	CHECK_THAT((quat1 * quat2).ComputeMatrix(), IsNearlyEqualToMatrix(quat1Mat * quat2Mat, std::numeric_limits<float>::epsilon() * 2));
	CHECK_THAT((quat3 * quat1).ComputeMatrix(), IsNearlyEqualToMatrix(quat3Mat * quat1Mat));

	CHECK_THAT(quat1.ComputeMatrix(), IsNearlyEqualToMatrix(Fl::Mat4f(1.f, 0.f, 0.f, 0.f,
		                                                              0.f, 0.98480773f, -0.173648179f, 0.f,
		                                                              0.f, 0.173648179f, 0.98480773f, 0.f,
		                                                              0.f, 0.f, 0.f, 1.f)));

	CHECK_THAT(quat2.ComputeMatrix(), IsNearlyEqualToMatrix(Fl::Mat4f(-0.870967627f, -0.45161289f, 0.193548396f, 0.f,
		                                                              0.193548396f, -0.677419245f, -0.709677398f, 0.f,
		                                                              0.45161289f, -0.580645144f, 0.677419424f, 0.f,
		                                                              0.f, 0.f, 0.f, 1.f)));

	CHECK_THAT(quat3.ComputeMatrix(), IsNearlyEqualToMatrix(Fl::Mat4f(-0.933333397f, -0.133333355f, 0.333333343f, 0.f,
		                                                              -0.133333325f, -0.733333409f, -0.666666687f, 0.f,
		                                                              0.333333343f, -0.666666687f, 0.666666627f, 0.f,
		                                                              0.f, 0.f, 0.f, 1.f)));
}

TEST_CASE("Quaternion: Vector multiplication", "[Math]") {
	// See: https://www.geogebra.org/m/aderzasm

	const Fl::Quaternionf quatRotX(45_deg, Fl::Axis::X);
	CHECK(quatRotX * Fl::Axis::X == Fl::Axis::X);
	CHECK(Fl::Axis::X * quatRotX == Fl::Axis::X);

	const Fl::Quaternionf quatRotY(45_deg, Fl::Axis::Y);
	CHECK(quatRotY * Fl::Axis::X == Fl::Vec3f(0.70710677f, 0.f, -0.70710677f));
	CHECK(Fl::Axis::X * quatRotY == Fl::Vec3f(0.70710677f, 0.f, 0.70710677f));

	const Fl::Quaternionf quatRotZ(45_deg, Fl::Axis::Z);
	CHECK(quatRotZ * Fl::Axis::X == Fl::Vec3f(0.70710677f, 0.70710677f, 0.f));
	CHECK(Fl::Axis::X * quatRotZ == Fl::Vec3f(0.70710677f, -0.70710677f, 0.f));

	CHECK(Fl::Axis::Z * Fl::Quaternionf(90_deg, Fl::Axis::Y) == -Fl::Axis::X);
	CHECK(Fl::Axis::Z * Fl::Mat3f(Fl::Quaternionf(90_deg, Fl::Axis::Y).ComputeMatrix()) == -Fl::Axis::X);

	constexpr Fl::Vec3f pos(0.123f, 14.51145f, 7.58413f);

	CHECK(quatRotX * pos == Fl::Vec3f(pos.X(), 4.89835405f, 15.6239338f));
	CHECK(quatRotY * pos == Fl::Vec3f(5.44976425f, pos.Y(), 5.27581501f));
	CHECK(quatRotZ * pos == Fl::Vec3f(-10.1741714f, 10.3481178f, pos.Z()));

	CHECK_THAT(pos * quatRotX, IsNearlyEqualToVector(Fl::Vec3f(pos.X(), 15.6239338f, -4.89835596f), 0.000001f));
	CHECK_THAT(pos * quatRotY, IsNearlyEqualToVector(Fl::Vec3f(-5.27581596f, pos.Y(), 5.4497633f)));
	CHECK_THAT(pos * quatRotZ, IsNearlyEqualToVector(Fl::Vec3f(10.3481188f, 10.1741686f, pos.Z())));

	// Checking that the matrix multiplication behaves the same way

	CHECK_THAT(quatRotX * pos, IsNearlyEqualToVector(Fl::Mat3f(quatRotX.ComputeMatrix()) * pos, 0.000001f));
	CHECK_THAT(quatRotY * pos, IsNearlyEqualToVector(Fl::Mat3f(quatRotY.ComputeMatrix()) * pos, 0.000001f));
	CHECK_THAT(quatRotZ * pos, IsNearlyEqualToVector(Fl::Mat3f(quatRotZ.ComputeMatrix()) * pos, 0.000001f));

	CHECK_THAT(pos * quatRotX, IsNearlyEqualToVector(pos * Fl::Mat3f(quatRotX.ComputeMatrix()), 0.000001f));
	CHECK_THAT(pos * quatRotY, IsNearlyEqualToVector(pos * Fl::Mat3f(quatRotY.ComputeMatrix()), 0.000001f));
	CHECK_THAT(pos * quatRotZ, IsNearlyEqualToVector(pos * Fl::Mat3f(quatRotZ.ComputeMatrix()), 0.000001f));
}

TEST_CASE("Quaternion: Quaternion multiplication", "[Math]") {
	constexpr Fl::Quaternionf unitQuat = Fl::Quaternionf::Identity();
	constexpr Fl::Mat4f unitQuatMat = unitQuat.ComputeMatrix();
	CHECK(unitQuatMat == Fl::Mat4f::Identity());

	constexpr Fl::Quaternionf squareUnitQuat = unitQuat * unitQuat;
	CHECK(squareUnitQuat.ComputeMatrix() == unitQuatMat);

	const Fl::Quaternionf quatRotX(45.0_deg, Fl::Axis::X);
	const Fl::Quaternionf multRotX = quatRotX * quatRotX.Conjugate();
	CHECK(multRotX.ComputeMatrix() == unitQuatMat);

	const Fl::Quaternionf quatRotY(45.0_deg, Fl::Axis::Y);
	const Fl::Quaternionf multRotY = quatRotY * quatRotY.Conjugate();
	CHECK(multRotY.ComputeMatrix() == unitQuatMat);

	const Fl::Quaternionf quatRotZ(45.0_deg, Fl::Axis::Z);
	const Fl::Quaternionf multRotZ = quatRotZ * quatRotZ.Conjugate();
	CHECK(multRotZ.ComputeMatrix() == unitQuatMat);

	const Fl::Quaternionf quat12 = quat1 * quat2;

	// Results taken from Wolfram Alpha: https://tinyurl.com/rxavw82
	CHECK(quat12.W() == 0.642787635f);
	CHECK(quat12.X() == 0.766044438f);
	CHECK(quat12.Y() == -1.716974139f);
	CHECK(quat12.Z() == 3.398823261f);
	CHECK_THAT(quat12.ComputeNorm(), IsNearlyEqualTo(3.9370039f));
	CHECK_THAT(quat12.ComputeMatrix(), IsNearlyEqualToMatrix(Fl::Mat4f(-0.870968f, -0.451613f, 0.1935484f, 0.f,
		                                                               0.1121862f, -0.5663f, -0.8165285f, 0.f,
		                                                               0.4783612f, -0.6894565f, 0.5438936f, 0.f,
		                                                               0.f, 0.f, 0.f, 1.f)));

	const Fl::Quaternionf quat21 = quat2 * quat1;

	// Results taken from Wolfram Alpha: https://tinyurl.com/426hupnb
	CHECK(quat21.W() == 0.642787635f);
	CHECK(quat21.X() == 0.766044438f);
	CHECK(quat21.Y() == -1.10069f);
	CHECK(quat21.Z() == 3.645337105f);
	CHECK_THAT(quat21.ComputeNorm(), IsNearlyEqualTo(3.9370039f));
	CHECK_THAT(quat21.ComputeMatrix(), IsNearlyEqualToMatrix(Fl::Mat4f(-0.870968f, -0.411142588f, 0.269029707f, 0.f,
		                                                               0.193548396f, -0.790362f, -0.581263244f, 0.f,
		                                                               0.451613f, -0.454191267f, 0.76795578f, 0.f,
		                                                               0.f, 0.f, 0.f, 1.f)));
}

TEST_CASE("Quaternion: Structured bindings", "[math]") {
	static_assert(std::tuple_size_v<decltype(quat1)> == 4);
	static_assert(std::tuple_size_v<decltype(Fl::Quaterniond::Identity())> == 4);

	static_assert(std::is_same_v<std::tuple_element_t<0, decltype(quat1)>, const float>);
	static_assert(std::is_same_v<std::tuple_element_t<2, decltype(Fl::Quaterniond::Identity())>, double>);
	static_assert(std::is_same_v<std::tuple_element_t<3, decltype(Fl::Quaternion<const double>::Identity())>, const double>);

	// When using structured bindings, Fl::get<I>(e) is exclusively found using ADL (https://en.cppreference.com/w/cpp/language/adl)
	CHECK(Fl::get<0>(quat1) == quat1.W());
	CHECK(Fl::get<1>(quat1) == quat1.X());
	CHECK(Fl::get<2>(quat1) == quat1.Y());
	CHECK(Fl::get<3>(quat1) == quat1.Z());

	{
		const auto [w, x, y, z] = quat1;
		static_assert(std::is_same_v<decltype(w), const float>);
		static_assert(std::is_same_v<decltype(x), const float>);
		static_assert(std::is_same_v<decltype(y), const float>);
		static_assert(std::is_same_v<decltype(z), const float>);
		// Only values are returned, never references
		static_assert(std::is_same_v<decltype(Fl::get<0>(quat1)), float>);
		static_assert(std::is_same_v<decltype(Fl::get<1>(quat1)), float>);
		static_assert(std::is_same_v<decltype(Fl::get<2>(quat1)), float>);
		static_assert(std::is_same_v<decltype(Fl::get<3>(quat1)), float>);

		CHECK(w == quat1.W());
		CHECK(x == quat1.X());
		CHECK(y == quat1.Y());
		CHECK(z == quat1.Z());
	}

	{
		constexpr Fl::Quaterniond quatDouble(1.0, 2.0, 3.0, 4.0);
		auto [w, x, y, z] = quatDouble;
		static_assert(std::is_same_v<decltype(w), double>);
		static_assert(std::is_same_v<decltype(x), double>);
		static_assert(std::is_same_v<decltype(y), double>);
		static_assert(std::is_same_v<decltype(z), double>);
		// Only values are returned, never references
		static_assert(std::is_same_v<decltype(Fl::get<0>(quatDouble)), double>);
		static_assert(std::is_same_v<decltype(Fl::get<1>(quatDouble)), double>);
		static_assert(std::is_same_v<decltype(Fl::get<2>(quatDouble)), double>);
		static_assert(std::is_same_v<decltype(Fl::get<3>(quatDouble)), double>);

		CHECK(w == quatDouble.W());
		CHECK(x == quatDouble.X());
		CHECK(y == quatDouble.Y());
		CHECK(z == quatDouble.Z());
	}

	// Not checking structured bindings with reference types, as quaternions never return any
}

TEST_CASE("Quaternion: Near-equality", "[Math]") {
	CHECK_FALSE(quat1 == quat2);

	constexpr Fl::Quaternionf baseQuat(1.f, 1.f, 1.f, 1.f);
	Fl::Quaternionf compQuat = baseQuat;

	CHECK(baseQuat.W() == compQuat.W()); // Copied, strict equality
	CHECK(baseQuat.X() == compQuat.X());
	CHECK(baseQuat.Y() == compQuat.Y());
	CHECK(baseQuat.Z() == compQuat.Z());

	compQuat = Fl::Quaternionf(baseQuat.W() + 0.0000001f,
		                        baseQuat.X() + 0.0000001f,
		                        baseQuat.Y() + 0.0000001f,
		                        baseQuat.Z() + 0.0000001f); // Adding a tiny offset

	CHECK_FALSE(baseQuat.W() == compQuat.W()); // Values not strictly equal
	CHECK_FALSE(baseQuat.X() == compQuat.X());
	CHECK_FALSE(baseQuat.Y() == compQuat.Y());
	CHECK_FALSE(baseQuat.Z() == compQuat.Z());

	CHECK_THAT(baseQuat.W(), IsNearlyEqualTo(compQuat.W())); // Near-equality components check
	CHECK_THAT(baseQuat.X(), IsNearlyEqualTo(compQuat.X()));
	CHECK_THAT(baseQuat.Y(), IsNearlyEqualTo(compQuat.Y()));
	CHECK_THAT(baseQuat.Z(), IsNearlyEqualTo(compQuat.Z()));

	CHECK_THAT(baseQuat, IsNearlyEqualToQuaternion(compQuat)); // Simpler check in a single call

	CHECK(baseQuat == compQuat); // Quaternion::operator== does a near-equality check
}

TEST_CASE("Quaternion: printing", "[Math]") {
	std::stringstream stream;

	stream << quat1;
	CHECK(stream.str() == "[ 0.996195, 0.0871557, 0, 0 ]");

	stream.str(std::string()); // Resetting the stream
	stream << quat2;
	CHECK(stream.str() == "[ 0.707107, 0.707107, -1.41421, 3.53553 ]");

	stream.str(std::string());
	stream << quat3;
	CHECK(stream.str() == "[ -4.37114e-08, 1, -2, 5 ]");
}