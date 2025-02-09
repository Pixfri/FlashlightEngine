// Copyright (C) 2025 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#include <FlashlightEngine/Math/Transform.hpp>

#include <FlashlightEngine/Core/Assert.hpp>
#include <FlashlightEngine/Math/Angle.hpp>
#include <FlashlightEngine/Math/Matrix.hpp>

namespace Fl {
	void Transform::SetPosition(const Vec3f& position) {
		m_Position = position;
		m_Updated = true;
	}

	void Transform::SetRotation(const Quaternionf& rotation) {
		m_Rotation = rotation;
		m_Updated = true;
	}
	
	void Transform::SetRotation(Radiansf angle, const Vec3f& axis) {
		SetRotation(Quaternionf(angle, axis));
	}

	void Transform::SetScale(const Vec3f& scale) {
		m_Scale = scale;
		m_Updated = true;
	}

	void Transform::Translate(const F32 x, const F32 y, const F32 z) {
		m_Position.X() += x;
		m_Position.Y() += y;
		m_Position.Z() += z;

		m_Updated = true;
	}

	void Transform::Rotate(const Quaternionf& rotation) {
		m_Rotation *= rotation;
		m_Updated = true;
	}

	void Transform::Rotate(const Radiansf angle, const Vec3f& axis) {
		FlAssert(FloatMath::AreNearlyEqual(axis.ComputeSquaredLength(), 1.f), "Rotation axis must be normalized.");
		Rotate(Quaternionf(angle, axis));
	}

	void Transform::Rotate(const Radiansf xAngle, const Radiansf yAngle) {
		const Quaternionf xQuat(xAngle, Axis::X);
		const Quaternionf yQuat(yAngle, Axis::Y);
		m_Rotation = yQuat * m_Rotation * xQuat;

		m_Updated = true;
	}

	void Transform::Rotate(const Radiansf xAngle, const Radiansf yAngle, const Radiansf zAngle) {
		const Quaternionf xQuat(xAngle, Axis::X);
		const Quaternionf yQuat(yAngle, Axis::Y);
		const Quaternionf zQuat(zAngle, Axis::Z);
		m_Rotation *= zQuat * xQuat * yQuat;

		m_Updated = true;
	}

	void Transform::Scale(const F32 x, const F32 y, const F32 z) {
		m_Scale.X() *= x;
		m_Scale.Y() *= y;
		m_Scale.Z() *= z;

		m_Updated = true;
	}

	Fl::Mat4f Transform::ComputeTranslationMatrix(const bool reverseTranslation) const {
		const Vec3f translation = reverseTranslation ? -m_Position : m_Position;
		const Mat4f translationMat(1.f, 0.f, 0.f, translation.X(),
			                       0.f, 1.f, 0.f, translation.Y(),
			                       0.f, 0.f, 1.f, translation.Z(),
			                       0.f, 0.f, 0.f, 1.f);

		return translationMat;
	}

	Fl::Mat4f Transform::ComputeTransformMatrix() const {
		const Mat4f scaleMat(m_Scale.X(), 0.0f,        0.f,         0.f,
			                 0.f,         m_Scale.Y(), 0.f,         0.f,
			                 0.f,         0.f,         m_Scale.Z(), 0.f,
			                 0.f,         0.f,         0.f,         1.f);

		return ComputeTranslationMatrix() * m_Rotation.ComputeMatrix() * scaleMat;
	}
}
