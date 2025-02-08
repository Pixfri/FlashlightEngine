// Copyright (C) 2025 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#ifndef FL_MATH_ANGLE_HPP
#define FL_MATH_ANGLE_HPP

#include <FlashlightEngine/Prerequisites.hpp>

#include <FlashlightEngine/Math/Constants.hpp>
#include <FlashlightEngine/Math/FloatMath.hpp>

namespace Fl {
    template <typename T>
    struct Degrees;

    template <typename T>
    struct Radians;

    /**
     * @brief Degrees class, handling mathematical angles in degrees.
     * @tparam T Type of the degrees' value.
     */
    template <typename T>
    struct Degrees {
        static_assert(std::is_floating_point_v<T>, "[Error] Fl::Degrees' type must be floating-point.");

        constexpr explicit Degrees(T val) noexcept;
        constexpr explicit Degrees(Radians<T> rad) noexcept;
        constexpr Degrees(const Degrees&) noexcept = default;
        constexpr Degrees(Degrees&&) noexcept = default;

        /**
         * @brief Default copy assignment operator.
         * @return Reference to the copied Fl::Degrees object.
         */
        constexpr Degrees& operator=(const Degrees&) noexcept = default;

        /**
         * @brief Default copy assignment operator.
         * @return Reference to the copied Fl::Degrees object.
         */
        constexpr Degrees& operator=(Degrees&&) noexcept = default;

        /**
         * @brief Degrees negation operator.
         * @return Negated degrees object.
         */
        constexpr Degrees operator-() const noexcept;

        /**
         * @brief Degrees-value addition operator.
         * @tparam ValueType Type of the value to be added.
         * @param val Value to be added.
         * @return Result of the value added to the Degrees object.
         */
        template <typename ValueType>
        constexpr Degrees operator+(ValueType val) const noexcept;

        /**
         * @brief Degrees-value subtraction operator.
         * @tparam ValueType Type of the value to be subtracted.
         * @param val Value to be subtracted.
         * @return Result of the value subtracted to the Degrees object.
         */
        template <typename ValueType>
        constexpr Degrees operator-(ValueType val) const noexcept;

        /**
         * @brief Degrees-value multiplication operator.
         * @tparam ValueType Type of the value to be multiplied by.
         * @param val Value to be multiplied by.
         * @return Result of the Degrees object multiplied by the value.
         */
        template <typename ValueType>
        constexpr Degrees operator*(ValueType val) const noexcept;

        /**
         * @brief Degrees-value division operator.
         * @tparam ValueType Type of the value to be divided by.
         * @param val Value to be divided by.
         * @return Result of the Degrees object divided by the value.
         */
        template <typename ValueType>
        constexpr Degrees operator/(ValueType val) const noexcept;

        /**
         * @brief Degrees-value addition assignment operator.
         * @tparam ValueType Type of the value to be added.
         * @param val Value to be added.
         * @return Reference to the modified Degrees object.
         */
        template <typename ValueType>
        constexpr Degrees& operator+=(ValueType val) noexcept;

        /**
         * @brief Degrees-value subtraction assignment operator.
         * @tparam ValueType Type of the value to be subtracted.
         * @param val Value to be subtracted.
         * @return Reference to the modified Degrees object.
         */
        template <typename ValueType>
        constexpr Degrees& operator-=(ValueType val) noexcept;

        /**
         * @brief Degrees-value multiplication assignment operator.
         * @tparam ValueType Type of the value to be multiplied by.
         * @param val Value to be multiplied by.
         * @return Reference to the modified Degrees object.
         */
        template <typename ValueType>
        constexpr Degrees& operator*=(ValueType val) noexcept;

        /**
         * @brief Degrees-value division assignment operator.
         * @tparam ValueType Type of the value to be divided by.
         * @param val Value to be divided by.
         * @return Reference to the modified Degrees object.
         */
        template <typename ValueType>
        constexpr Degrees& operator/=(ValueType val) noexcept;

        /**
         * @brief Checks if the current degrees angle is equal to another given one. Uses a near equality check to take
         *        floating-point errors into account.
         * @param deg Degrees object to be compared with.
         * @return Whether both Degrees objects are nearly equal or not.
         */
        constexpr bool operator==(const Degrees& deg) const noexcept;

        /**
         * @brief Checks if the current degrees angle is not equal to another given one. Uses a near equality check to
         *        take floating-point errors into account.
         * @param deg Degrees object to be compared with.
         * @return Whether both Degrees objects are not equal or if they are.
         */
        constexpr bool operator!=(const Degrees& deg) const noexcept;

        /**
         * @brief Implicit degrees' value type conversion operator.
         * @tparam To Type to convert the value to.
         * @return Degrees object of the new type.
         */
        template <typename To>
        constexpr operator Degrees<To>() const noexcept;

        /**
         * @brief Implicit degrees to radians conversion operator.
         * @tparam To Type to convert the value to.
         * @return Radians object of the new type.
         */
        template <typename To>
        constexpr operator Radians<To>() const noexcept;

        T Value;
    };

    // Aliases

    using Degreesf = Degrees<F32>;
    using Degreesd = Degrees<F64>;
    using Degreesld = Degrees<long double>;

    /**
     * @brief Radians class, handling mathematical angles in radians.
     * @tparam T Type of the degrees' value.
     */
    template <typename T>
    struct Radians {
        static_assert(std::is_floating_point_v<T>, "[Error] Fl::Radians' type must be floating-point.");

        constexpr explicit Radians(T val) noexcept;
        constexpr explicit Radians(Degrees<T> deg) noexcept;
        constexpr Radians(const Radians&) noexcept = default;
        constexpr Radians(Radians&&) noexcept = default;

        /**
         * @brief Default copy assignment operator.
         * @return Reference to the copied Fl::Radians object.
         */
        constexpr Radians& operator=(const Radians&) noexcept = default;

        /**
         * @brief Default copy assignment operator.
         * @return Reference to the copied Fl::Radians object.
         */
        constexpr Radians& operator=(Radians&&) noexcept = default;

        /**
         * @brief Radians negation operator.
         * @return Negated radians object.
         */
        constexpr Radians operator-() const noexcept;

        /**
         * @brief Radians-value addition operator.
         * @tparam ValueType Type of the value to be added.
         * @param val Value to be added.
         * @return Result of the value added to the Radians object.
         */
        template <typename ValueType>
        constexpr Radians operator+(ValueType val) const noexcept;

        /**
         * @brief Radians-value subtraction operator.
         * @tparam ValueType Type of the value to be subtracted.
         * @param val Value to be subtracted.
         * @return Result of the value subtracted to the Radians object.
         */
        template <typename ValueType>
        constexpr Radians operator-(ValueType val) const noexcept;

        /**
         * @brief Radians-value multiplication operator.
         * @tparam ValueType Type of the value to be multiplied by.
         * @param val Value to be multiplied by.
         * @return Result of the Radians object multiplied by the value.
         */
        template <typename ValueType>
        constexpr Radians operator*(ValueType val) const noexcept;

        /**
         * @brief Radians-value division operator.
         * @tparam ValueType Type of the value to be divided by.
         * @param val Value to be divided by.
         * @return Result of the Radians object divided by the value.
         */
        template <typename ValueType>
        constexpr Radians operator/(ValueType val) const noexcept;

        /**
         * @brief Radians-value addition assignment operator.
         * @tparam ValueType Type of the value to be added.
         * @param val Value to be added.
         * @return Reference to the modified Radians object.
         */
        template <typename ValueType>
        constexpr Radians& operator+=(ValueType val) noexcept;

        /**
         * @brief Radians-value subtraction assignment operator.
         * @tparam ValueType Type of the value to be subtracted.
         * @param val Value to be subtracted.
         * @return Reference to the modified Radians object.
         */
        template <typename ValueType>
        constexpr Radians& operator-=(ValueType val) noexcept;

        /**
         * @brief Radians-value multiplication assignment operator.
         * @tparam ValueType Type of the value to be multiplied by.
         * @param val Value to be multiplied by.
         * @return Reference to the modified Radians object.
         */
        template <typename ValueType>
        constexpr Radians& operator*=(ValueType val) noexcept;

        /**
         * @brief Radians-value division assignment operator.
         * @tparam ValueType Type of the value to be divided by.
         * @param val Value to be divided by.
         * @return Reference to the modified Radians object.
         */
        template <typename ValueType>
        constexpr Radians& operator/=(ValueType val) noexcept;

        /**
         * @brief Checks if the current radians angle is equal to another given one. Uses a near equality check to take
         *        floating-point errors into account.
         * @param rad Radians object to be compared with.
         * @return Whether both Radians objects are nearly equal or not.
         */
        constexpr bool operator==(const Radians& rad) const noexcept;

        /**
         * @brief Checks if the current radians angle is not equal to another given one. Uses a near equality check to
         *        take floating-point errors into account.
         * @param rad Radians object to be compared with.
         * @return Whether both Radians objects are not equal or if they are.
         */
        constexpr bool operator!=(const Radians& rad) const noexcept;

        /**
         * @brief Implicit radians' value type conversion operator.
         * @tparam To Type to convert the value to.
         * @return Radians object of the new type.
         */
        template <typename To>
        constexpr operator Radians<To>() const noexcept;

        /**
         * @brief Implicit radians to degrees conversion operator.
         * @tparam To Type to convert the value to.
         * @return Degrees object of the new type.
         */
        template <typename To>
        constexpr operator Degrees<To>() const noexcept;

        T Value;
    };

    // Aliases

    using Radiansf = Radians<F32>;
    using Radiansd = Radians<F64>;
    using Radiansld = Radians<long double>;

    namespace Literals {
        constexpr Degreesld operator ""_deg(const U64 val) noexcept {
            return Degreesld(static_cast<long double>(val));
        }

        constexpr Degreesld operator ""_deg(const long double val) noexcept {
            return Degreesld(val);
        }

        constexpr Radiansld operator ""_rad(const U64 val) noexcept {
            return Radiansld(static_cast<long double>(val));
        }

        constexpr Radiansld operator ""_rad(const long double val) noexcept {
            return Radiansld(val);
        }
    }
}

#include <FlashlightEngine/Math/Angle.inl>

#endif // FL_MATH_ANGLE_HPP
