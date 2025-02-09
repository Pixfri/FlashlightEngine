// Copyright (C) 2025 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

namespace Fl {
    inline explicit Transform::Transform(const Vec3f& position, const Quaternionf& rotation, const Vec3f& scale)
        : m_Position{ position }, m_Rotation{ rotation }, m_Scale{ scale } {
	}

	inline const Vec3f& Transform::GetPosition() const {
		return m_Position;
	}

	inline const Quaternionf& Transform::GetRotation() const {
		return m_Rotation;
	}

	inline const Vec3f& Transform::GetScale() const {
		return m_Scale;
	}

	inline bool Transform::WasUpdated() const {
		return m_Updated;
	}

	inline void Transform::SetPosition(const F32 x, const F32 y, const F32 z) {
		SetPosition(Vec3f(x, y, z));
	}

	inline void Transform::SetScale(const F32 x, const F32 y, const F32 z) {
		SetScale(Vec3f(x, y, z));
	}

	inline void Transform::SetScale(const F32 coeff) {
		SetScale(Vec3f(coeff));
	}

	inline void Transform::SetUpdated(const bool status) {
		m_Updated = status;
	}

	inline void Transform::Move(const Vec3f& displacement) {
		Translate(m_Rotation * displacement);
	}

	inline void Transform::Move(const F32 x, const F32 y, const F32 z) {
		Move(Vec3f(x, y, z));
	}

	inline void Transform::Translate(const Vec3f& displacement) {
		Translate(displacement.X(), displacement.Y(), displacement.Z());
	}

	inline void Transform::Scale(const Vec3f& values) {
		Scale(values.X(), values.Y(), values.Z());
	}

	inline void Transform::Scale(const F32 coeff) {
		Scale(coeff, coeff, coeff);
	}
}
