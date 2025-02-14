// Copyright (C) 2025 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#ifndef FL_MATH_TRANSFORM_HPP
#define FL_MATH_TRANSFORM_HPP

#include <FlashlightEngine/Prerequisites.hpp>

#include <FlashlightEngine/Core/Component.hpp>
#include <FlashlightEngine/Math/Quaternion.hpp>
#include <FlashlightEngine/Math/Vector.hpp>

namespace Fl {
    template <typename T, U64 Width, U64 Height>
    class Matrix;
    using Mat4f = Matrix<F32, 4, 4>;

    template <typename T>
    struct Radians;
    using Radiansf = Radians<F32>;

    /**
     * @brief Transform entity component which handles transformations in a 3D space
     */
    class Transform : public Component {
    public:
        inline explicit Transform(const Vec3f& position = Vec3f(0.f), const Quaternionf& rotation = Quaternionf::Identity(), const Vec3f& scale = Vec3f(1.f));

        inline const Vec3f& GetPosition() const;
        inline const Quaternionf& GetRotation() const;
        inline const Vec3f& GetScale() const;
        inline bool WasUpdated() const;

        void SetPosition(const Vec3f& position);
        inline void SetPosition(F32 x, F32 y, F32 z);
        void SetRotation(const Quaternionf& rotation);
        void SetRotation(Radiansf angle, const Vec3f& axis);
        void SetScale(const Vec3f& scale);
        inline void SetScale(F32 x, F32 y, F32 z);
        inline void SetScale(F32 coeff);
        inline void SetUpdated(bool status);

        /**
         * @brief Moves by the given values in relative coordinates (takes rotation into account).
         * @param displacement Displacement to be moved by.
         */
        inline void Move(const Vec3f& displacement);
        /**
         * @brief Moves by the given values in relative coordinates (takes rotation into account).
		 * @param x Value to be moved by on the X axis.
		 * @param y Value to be moved by on the Y axis.
         * @param Z Value to be moved by on the Z axis.
         */
        inline void Move(F32 x, F32 y, F32 z);
        /**
         * @brief Translates by the given values in absolute coordinates (does not take rotation into account).
         * @param displacement Displacement to be translated by.
         */
        inline void Translate(const Vec3f& displacement);
        /**
         * @brief Translates by the given values in absolute coordinates (does not take rotation into account).
		 * @param x Value to be translated by on the X axis.
		 * @param y Value to be translated by on the Y axis.
		 * @param Z Value to be translated by on the Z axis.
		 */
        void Translate(F32 x, F32 y, F32 z);
        /**
         * @brief Rotates by the given quaternion.
         * @param rotation Rotation quaternion to rotate by.
         */
        void Rotate(const Quaternionf& rotation);
        /**
         * @brief Rotates by the given angle around the given axis.
         * @note The axis must be a normalized vector for this function to work properly.
         * @param angle Angle to rotate by.
         * @param axis Axis to rotate around.
         */
        void Rotate(Radiansf angle, const Vec3f& axis);
        /**
         * @brief Rotates around the given axes. This locks the up vector, preventing any rolling effect.
         * @param xAngle Value of X to rotate around.
         * @param yAngle Value of Y to rotate around.
         */
		void Rotate(Radiansf xAngle, Radiansf yAngle);
        /**
         * @brief Rotates around the given axes.
         * @param xAngle Value of X to rotate around.
         * @param yAngle Value of Y to rotate around.
         * @param zAngle Value of Z to rotate around.
         */
		void Rotate(Radiansf xAngle, Radiansf yAngle, Radiansf zAngle);
        /**
         * @brief Scales by the given values.
         *        The scaling is a coefficient: scaling by a value of 2 doubles the size, while a value of 0.5 shrinks it by half.
         * @param values Values to be scaled by.
         */
        inline void Scale(const Vec3f& values);
        /**
         * @brief Scales by the given values.
         *        The scaling is a coefficient: scaling by a value of 2 doubles the size, while a value of 0.5 shrinks it by half.
		 * @param x Value to be scaled by on the X axis.
		 * @param y Value to be scaled by on the Y axis.
		 * @param z Value to be scaled by on the Z axis.
         */
        void Scale(F32 x, F32 y, F32 z);
        /**
         * @brief Scales by the given value on all 3 axes.
         *        The scaling is a coefficient: scaling by a value of 2 doubles the size, while a value of 0.5 shrinks it by half.
         * @param coeff Coefficient to be scaled by on all axes.
         */
        inline void Scale(F32 coeff);
        /**
         * @brief Computes the translation matrix (identity matrix with the translation in the last column).
         * @param reverseTranslation Whether the translation should be reversed (negated) or not.
         * @return Translation matrix.
         */
        Mat4f ComputeTranslationMatrix(bool reverseTranslation = false) const;
        /**
         * @brief Computes the transformation matrix.
         *        This matrix combines all three features: translation, rotation & scale.
         * @return Transformation matrix.
         */
        Mat4f ComputeTransformMatrix() const;

    private:
        Vec3f m_Position{};
        Quaternionf m_Rotation = Quaternionf::Identity();
        Vec3f m_Scale{};
        bool m_Updated{true};
    };
}

#include <FlashlightEngine/Math/Transform.inl>

#endif // FL_MATH_TRANSFORM_HPP
