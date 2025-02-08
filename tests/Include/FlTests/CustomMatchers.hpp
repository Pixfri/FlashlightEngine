// Copyright (C) 2025 Jean "Pixfri" Letessier
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#ifndef FL_TESTS_CUSTOMMATCHERS_HPP
#define FL_TESTS_CUSTOMMATCHERS_HPP

#include <FlashlightEngine/Math/FloatMath.hpp>

#include <catch2/matchers/catch_matchers.hpp>

#include <sstream>

/**
 * @brief Custom Catch matcher, which checks for near-equality between floating point values.
 * @tparam T Type of the value to be compared to.
 * @tparam ToleranceType Tolerance type, which may differ from the value type.
 */
template <typename T, typename ToleranceType = T>
class IsNearlyEqualTo final : public Catch::Matchers::MatcherBase<T> {
    static_assert(std::is_floating_point_v<T>, "[Error] IsNearlyEqualTo's value type must be floating-point.");
    static_assert(std::is_floating_point_v<ToleranceType>,
                  "[Error] IsNearlyEqualTo's tolerance type must be floating-point.");

public:
    /**
     * @brief Creates an instance of a near-equality check custom matcher.
     * @param comparison Value to be compared with.
     * @param absoluteTolerance Absolute tolerance to compare the values with.
     */
    constexpr explicit IsNearlyEqualTo(T comparison,
                                       ToleranceType absoluteTolerance = std::numeric_limits<ToleranceType>::epsilon());

    /**
     * @brief Checks if the given value is nearly equal to the comparison one.
     * @param base Base value to compare to.
     * @return True if values are nearly equal to each other, false otherwise.
     */
    constexpr bool match(const T& base) const override;

    /**
     * @brief Gets the description of the error if the match failed.
     * @return Error string to be printed.
     */
    std::string describe() const override;

private:
    T m_Comparison;
    ToleranceType m_AbsoluteTolerance;
};

template <typename T, typename ToleranceType>
IsNearlyEqualTo(T, ToleranceType) -> IsNearlyEqualTo<T, ToleranceType>;

/**
 * @brief Custom Catch matcher, which checks for near-equality between floating point vectors' values.
 * @tparam T Type of the vector to be compared to.
 * @tparam ToleranceType Tolerance type, which may differ from the value type.
 */
template <typename T, Fl::U64 Size, typename ToleranceType = T>
class IsNearlyEqualToVector final : public Catch::Matchers::MatcherBase<Fl::Vector<T, Size>> {
    static_assert(std::is_floating_point_v<T>, "[Error] IsNearlyEqualToVector's value type must be floating-point.");
    static_assert(std::is_floating_point_v<ToleranceType>,
                  "[Error] IsNearlyEqualToVector's tolerance type must be floating-point.");

public:
    /**
     * @brief Creates an instance of a near-equality vector check custom matcher.
     * @param comparison Vector to be compared with.
     * @param absoluteTolerance Absolute tolerance to compare the values with.
     */
    constexpr explicit IsNearlyEqualToVector(const Fl::Vector<T, Size>& comparison,
                                             ToleranceType absoluteTolerance
                                             = std::numeric_limits<ToleranceType>::epsilon());

    /**
     * @brief Checks if the given vector has nearly equal values compared to the comparison one.
     * @param base Base vector to compare to.
     * @return True if values are nearly equal to each other, false otherwise.
     */
    constexpr bool match(const Fl::Vector<T, Size>& base) const override;

    /**
     * @brief Gets the description of the error if the match failed.
     * @return Error string to be printed.
     */
    std::string describe() const override;

private:
    Fl::Vector<T, Size> m_Base;
    Fl::Vector<T, Size> m_Comparison;
    ToleranceType m_AbsoluteTolerance;
};

template <typename T, Fl::U64 Size, typename ToleranceType>
IsNearlyEqualToVector(Fl::Vector<T, Size>, ToleranceType) -> IsNearlyEqualToVector<T, Size, ToleranceType>;

/**
 * @brief Custom Catch matcher, which checks for near-equality between floating point matrices' values.
 * @tparam T Type of the matrix to be compared to.
 * @tparam Width Width of the matrices to be compared.
 * @tparam Height Height of the matrices to be compared.
 * @tparam ToleranceType Tolerance type, which may differ from the value type.
 */
template <typename T, Fl::U64 Width, Fl::U64 Height, typename ToleranceType = T>
class IsNearlyEqualToMatrix final : public Catch::Matchers::MatcherBase<Fl::Matrix<T, Width, Height>> {
    static_assert(std::is_floating_point_v<T>, "[Error] IsNearlyEqualTo's value type must be floating-point.");
    static_assert(std::is_floating_point_v<ToleranceType>,
                  "[Error] IsNearlyEqualTo's tolerance type must be floating-point.");

public:
    /**
     * @brief Creates an instance of a near-equality matrix check custom matcher.
     * @param comparison Matrix to be compared with.
     * @param absoluteTolerance Absolute tolerance to compare the values with.
     */
    constexpr explicit IsNearlyEqualToMatrix(const Fl::Matrix<T, Width, Height>& comparison,
                                             ToleranceType absoluteTolerance
                                             = std::numeric_limits<ToleranceType>::epsilon());

    /**
     * @brief Checks if the given matrix has nearly equal values compared to the comparison one.
     * @param base Base matrix to compare to.
     * @return True if values are nearly equal to each other, false otherwise.
     */
    constexpr bool match(const Fl::Matrix<T, Width, Height>& base) const override;

    /**
     * @brief Gets the description of the error if the match failed.
     * @return Error string to be printed.
     */
    std::string describe() const override;

private:
    Fl::Matrix<T, Width, Height> m_Base;
    Fl::Matrix<T, Width, Height> m_Comparison;
    ToleranceType m_AbsoluteTolerance;
};

template <typename T, Fl::U64 Width, Fl::U64 Height, typename ToleranceType = T>
IsNearlyEqualToMatrix(Fl::Matrix<T, Width, Height>, ToleranceType) -> IsNearlyEqualToMatrix<T, Width, Height, ToleranceType>;

/**
 * @brief Custom Catch matcher, which checks for near-equality between floating point quaternions' values.
 * @tparam T Type of the quaternion to be compared to.
 * @tparam ToleranceType Tolerance type, which may differ from the value type.
 */
template <typename T, typename ToleranceType = T>
class IsNearlyEqualToQuaternion final : public Catch::Matchers::MatcherBase<Fl::Quaternion<T>> {
    static_assert(std::is_floating_point_v<T>, "[Error] IsNearlyEqualTo's value type must be floating-point.");
    static_assert(std::is_floating_point_v<ToleranceType>,
                  "[Error] IsNearlyEqualTo's tolerance type must be floating-point.");

public:
    /**
     * @brief Creates an instance of a near-equality quaternion check custom matcher.
     * @param comparison Quaternion to be compared with.
     * @param absoluteTolerance Absolute tolerance to compare the values with.
     */
    constexpr explicit IsNearlyEqualToQuaternion(const Fl::Quaternion<T>& comparison,
                                                 ToleranceType absoluteTolerance = std::numeric_limits<ToleranceType>::epsilon());

    /**
     * @brief Checks if the given value is nearly equal to the comparison one.
     * @param base Base value to compare to.
     * @return True if values are nearly equal to each other, false otherwise.
     */
    constexpr bool match(const Fl::Quaternion<T>& base) const override;

    /**
     * @brief Gets the description of the error if the match failed.
     * @return Error string to be printed.
     */
    std::string describe() const override;

private:
    // m_Base needs an instantiation, since a Fl::Quaternion doesn't have a default constructor
    Fl::Quaternion<T> m_Base = Fl::Quaternion<T>::Identity();
    Fl::Quaternion<T> m_Comparison;
    ToleranceType m_AbsoluteTolerance;
};

template <typename T, typename ToleranceType>
IsNearlyEqualToQuaternion(Fl::Quaternion<T>, ToleranceType) -> IsNearlyEqualToQuaternion<T, ToleranceType>;

#include <FlTests/CustomMatchers.inl>

#endif // FL_TESTS_CUSTOMMATCHERS_HPP
