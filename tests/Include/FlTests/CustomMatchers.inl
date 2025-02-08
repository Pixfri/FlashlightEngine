// Copyright (C) 2025 Jean "Pixfri" Letessier
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

template <typename T, typename ToleranceType>
constexpr IsNearlyEqualTo<T, ToleranceType>::IsNearlyEqualTo(T comparison, ToleranceType absoluteTolerance)
    : m_Comparison(comparison), m_AbsoluteTolerance(absoluteTolerance) {
}

template <typename T, typename ToleranceType>
constexpr bool IsNearlyEqualTo<T, ToleranceType>::match(const T& base) const {
    return Fl::FloatMath::AreNearlyEqual(base, m_Comparison, m_AbsoluteTolerance);
}

template <typename T, typename ToleranceType>
std::string IsNearlyEqualTo<T, ToleranceType>::describe() const {
    std::ostringstream stream;
    stream.precision(std::numeric_limits<T>::digits10 + 3);

    stream << "is not nearly equal to " << m_Comparison;
    return stream.str();
}

template <typename T, Fl::U64 Size, typename ToleranceType>
constexpr IsNearlyEqualToVector<T, Size, ToleranceType>::IsNearlyEqualToVector(const Fl::Vector<T, Size>& comparison,
                                                                               ToleranceType absoluteTolerance)
    : m_Comparison{comparison}, m_AbsoluteTolerance{absoluteTolerance} {
}

template <typename T, Fl::U64 Size, typename ToleranceType>
constexpr bool IsNearlyEqualToVector<T, Size, ToleranceType>::match(const T& base) const {
    const_cast<IsNearlyEqualToVector*>(this)->m_Base = base;

    return Fl::FloatMath::AreNearlyEqual(m_Base, m_Comparison, m_AbsoluteTolerance);
}

template <typename T, Fl::U64 Size, typename ToleranceType>
std::string IsNearlyEqualToVector<T, Size, ToleranceType>::describe() const {
    std::ostringstream stream;
    stream.precision(std::numeric_limits<T>::digits10 + 3);

    for (std::size_t i = 0; i < Size; ++i) {
        if (!Fl::FloatMath::AreNearlyEqual(m_Base[i], m_Comparison[i], m_AbsoluteTolerance)) {
            stream << "\n\tAt [" << i << "]: " << m_Base[i] << " is not nearly equal to " << m_Comparison[i];
        }
    }

    return stream.str();
}

template <typename T, Fl::U64 Width, Fl::U64 Height, typename ToleranceType>
constexpr IsNearlyEqualToMatrix<T, Width, Height, ToleranceType>::IsNearlyEqualToMatrix(
    const Fl::Matrix<T, Width, Height>& comparison, ToleranceType absoluteTolerance)
    : m_Comparison{comparison}, m_AbsoluteTolerance{absoluteTolerance} {
}

template <typename T, Fl::U64 Width, Fl::U64 Height, typename ToleranceType>
constexpr bool IsNearlyEqualToMatrix<T, Width, Height, ToleranceType>::match(const T& base) const {
    const_cast<IsNearlyEqualToMatrix*>(this)->m_base = base;

    return Fl::FloatMath::AreNearlyEqual(m_Base, m_Comparison, m_AbsoluteTolerance);
}

template <typename T, Fl::U64 Width, Fl::U64 Height, typename ToleranceType>
std::string IsNearlyEqualToMatrix<T, Width, Height, ToleranceType>::describe() const {
    std::ostringstream stream;
    stream.precision(std::numeric_limits<T>::digits10 + 3);

    for (std::size_t widthIndex = 0; widthIndex < Width; ++widthIndex) {
        const std::size_t finalWidthIndex = widthIndex * Height;

        for (std::size_t heightIndex = 0; heightIndex < Height; ++heightIndex) {
            const std::size_t finalIndex = finalWidthIndex + heightIndex;

            if (!Fl::FloatMath::AreNearlyEqual(m_Base[finalIndex], m_Comparison[finalIndex], m_AbsoluteTolerance)) {
                stream << "\n\tAt [" << widthIndex << "][" << heightIndex << "]: " << m_Base[finalIndex]
                << " is not nearly equal to " << m_Comparison[finalIndex];
            }
        }
    }

    return stream.str();
}

template <typename T, typename ToleranceType>
constexpr IsNearlyEqualToQuaternion<T, ToleranceType>::IsNearlyEqualToQuaternion(const Fl::Quaternion<T>& comparison,
                                                                                 ToleranceType absoluteTolerance)
    : m_Comparison{comparison}, m_AbsoluteTolerance{absoluteTolerance} {
}

template <typename T, typename ToleranceType>
constexpr bool IsNearlyEqualToQuaternion<T, ToleranceType>::match(const Fl::Quaternion<T>& base) const {
    const_cast<IsNearlyEqualToQuaternion*>(this)->m_Base = base;

    return Fl::FloatMath::AreNearlyEqual(m_Base, m_Comparison, m_AbsoluteTolerance);
}

template <typename T, typename ToleranceType>
std::string IsNearlyEqualToQuaternion<T, ToleranceType>::describe() const {
    std::ostringstream stream;
    stream.precision(std::numeric_limits<T>::digits10 + 3);

    if (!Fl::FloatMath::AreNearlyEqual(m_Base.W(), m_Comparison.W(), m_AbsoluteTolerance)) {
        stream << "\n\tOn w: " << m_Base.W() << " is not nearly equal to " << m_Comparison.W();
    }

    if (!Fl::FloatMath::AreNearlyEqual(m_Base.X(), m_Comparison.X(), m_AbsoluteTolerance)) {
        stream << "\n\tOn x: " << m_Base.X() << " is not nearly equal to " << m_Comparison.X();
    }

    if (!Fl::FloatMath::AreNearlyEqual(m_Base.Y(), m_Comparison.Y(), m_AbsoluteTolerance)) {
        stream << "\n\tOn y: " << m_Base.Y() << " is not nearly equal to " << m_Comparison.Y();
    }

    if (!Fl::FloatMath::AreNearlyEqual(m_Base.Z(), m_Comparison.Z(), m_AbsoluteTolerance)) {
        stream << "\n\tOn z: " << m_Base.Z() << " is not nearly equal to " << m_Comparison.Z();
    }

    return stream.str();
}
