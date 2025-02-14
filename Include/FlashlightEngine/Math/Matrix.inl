// Copyright (C) 2025 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#include <FlashlightEngine/Core/Assert.hpp>
#include <FlashlightEngine/Math/FloatMath.hpp>

#include <algorithm>


namespace Fl {
    namespace Detail {
        // clang-format off
        // @formatter:off
        template <typename T>
        constexpr T ComputeMatrixDeterminant(const Mat2<T>& mat) {
            return (mat[0] * mat[3]) - (mat[2] * mat[1]);
        }

        template <typename T>
        constexpr T ComputeMatrixDeterminant(const Mat3<T>& mat) {
            const Mat2<T> leftMatrix(mat[4], mat[5],
                                     mat[7], mat[8]);

            const Mat2<T> centerMatrix(mat[3], mat[5],
                                       mat[6], mat[8]);


            const Mat2<T> rightMatrix(mat[3], mat[4],
                                      mat[6], mat[7]);

            return ComputeMatrixDeterminant(leftMatrix)   * mat[0]
                 - ComputeMatrixDeterminant(centerMatrix) * mat[1]
                 + ComputeMatrixDeterminant(rightMatrix)  * mat[2];
        }

        template <typename T>
        constexpr T ComputeMatrixDeterminant(const Mat4<T>& mat) noexcept {
            const Mat3<T> leftMatrix(mat[5],  mat[6],  mat[7],
                                     mat[9],  mat[10], mat[11],
                                     mat[13], mat[14], mat[15]);

            const Mat3<T> centerLeftMatrix(mat[4],  mat[6],  mat[7],
                                           mat[8],  mat[10], mat[11],
                                           mat[12], mat[14], mat[15]);

            const Mat3<T> centerRightMatrix(mat[4],  mat[5],  mat[7],
                                            mat[8],  mat[9],  mat[11],
                                            mat[12], mat[13], mat[15]);

            const Mat3<T> rightMatrix(mat[4],  mat[5],  mat[6],
                                      mat[8],  mat[9],  mat[10],
                                      mat[12], mat[13], mat[14]);

            return ComputeMatrixDeterminant(leftMatrix)        * mat[0]
                 - ComputeMatrixDeterminant(centerLeftMatrix)  * mat[1]
                 + ComputeMatrixDeterminant(centerRightMatrix) * mat[2]
                 - ComputeMatrixDeterminant(rightMatrix)       * mat[3];
        }

        template <typename T>
        constexpr Mat2<T> ComputeMatrixInverse(const Mat2<T>& mat, T invDeterminant) noexcept {
            return Mat2<T>(
                 mat[3] * invDeterminant, -mat[1] * invDeterminant,
                -mat[2] * invDeterminant,  mat[0] * invDeterminant
            );
        }

        template <typename T>
        constexpr Mat3<T> ComputeMatrixInverse(const Mat3<T>& mat, T invDeterminant) noexcept {
            const Mat2<T> topLeft(mat[4], mat[5],
                                  mat[7], mat[8]);
            const Mat2<T> topCenter(mat[3], mat[5],
                                    mat[6], mat[8]);
            const Mat2<T> topRight(mat[3], mat[4],
                                   mat[6], mat[7]);

            const Mat2<T> midLeft(mat[1], mat[2],
                                  mat[7], mat[8]);
            const Mat2<T> midCenter(mat[0], mat[2],
                                    mat[6], mat[8]);
            const Mat2<T> midRight(mat[0], mat[1],
                                   mat[6], mat[7]);

            const Mat2<T> botLeft(mat[1], mat[2],
                                  mat[4], mat[5]);
            const Mat2<T> botCenter(mat[0], mat[2],
                                    mat[3], mat[5]);
            const Mat2<T> botRight(mat[0], mat[1],
                                   mat[3], mat[4]);

            const Mat3<T> cofactors(
                 topLeft.ComputeDeterminant(), -topCenter.ComputeDeterminant(),  topRight.ComputeDeterminant(),
                -midLeft.ComputeDeterminant(),  midCenter.ComputeDeterminant(), -midRight.ComputeDeterminant(),
                 botLeft.ComputeDeterminant(), -botCenter.ComputeDeterminant(),  botRight.ComputeDeterminant()
            );

            return cofactors * invDeterminant;
        }

        template <typename T>
        constexpr Mat4<T> ComputeMatrixInverse(const Mat4<T>& mat, T invDeterminant) noexcept {
            const Mat3<T> topLeft(mat[5],  mat[6],  mat[7],
                                  mat[9],  mat[10], mat[11],
                                  mat[13], mat[14], mat[15]);
            const Mat3<T> topCenterLeft(mat[4],  mat[6],  mat[7],
                                        mat[8],  mat[10], mat[11],
                                        mat[12], mat[14], mat[15]);
            const Mat3<T> topCenterRight(mat[4],  mat[5],  mat[7],
                                         mat[8],  mat[9],  mat[11],
                                         mat[12], mat[13], mat[15]);
            const Mat3<T> topRight(mat[4],  mat[5],  mat[6],
                                   mat[8],  mat[9],  mat[10],
                                   mat[12], mat[13], mat[14]);

            const Mat3<T> midTopLeft(mat[1],  mat[2],  mat[3],
                                     mat[9],  mat[10], mat[11],
                                     mat[13], mat[14], mat[15]);
            const Mat3<T> midTopCenterLeft(mat[0],  mat[2],  mat[3],
                                           mat[8],  mat[10], mat[11],
                                           mat[12], mat[14], mat[15]);
            const Mat3<T> midTopCenterRight(mat[0],  mat[1],  mat[3],
                                            mat[8],  mat[9],  mat[11],
                                            mat[12], mat[13], mat[15]);
            const Mat3<T> midTopRight(mat[0],  mat[1],  mat[2],
                                      mat[8],  mat[9],  mat[10],
                                      mat[12], mat[13], mat[14]);

            const Mat3<T> midBotLeft(mat[1],  mat[2],  mat[3],
                                     mat[5],  mat[6],  mat[7],
                                     mat[13], mat[14], mat[15]);
            const Mat3<T> midBotCenterLeft(mat[0],  mat[2],  mat[3],
                                           mat[4],  mat[6],  mat[7],
                                           mat[12], mat[14], mat[15]);
            const Mat3<T> midBotCenterRight(mat[0],  mat[1],  mat[3],
                                            mat[4],  mat[5],  mat[7],
                                            mat[12], mat[13], mat[15]);
            const Mat3<T> midBotRight(mat[0],  mat[1],  mat[2],
                                      mat[4],  mat[5],  mat[6],
                                      mat[12], mat[13], mat[14]);

            const Mat3<T> botLeft(mat[1], mat[2], mat[3],
                                  mat[5], mat[6], mat[7],
                                  mat[9], mat[10], mat[11]);
            const Mat3<T> botCenterLeft(mat[0], mat[2], mat[3],
                                        mat[4], mat[6], mat[7],
                                        mat[8], mat[10], mat[11]);
            const Mat3<T> botCenterRight(mat[0], mat[1], mat[3],
                                         mat[4], mat[5], mat[7],
                                         mat[8], mat[9], mat[11]);
            const Mat3<T> botRight(mat[0], mat[1], mat[2],
                                   mat[4], mat[5], mat[6],
                                   mat[8], mat[9], mat[10]);

            const T topLeftDeterm        = topLeft.ComputeDeterminant();
            const T topCenterLeftDeterm  = topCenterLeft.ComputeDeterminant();
            const T topCenterRightDeterm = topCenterRight.ComputeDeterminant();
            const T topRightDeterm       = topRight.ComputeDeterminant();

            const T midTopLeftDeterm        = midTopLeft.ComputeDeterminant();
            const T midTopCenterLeftDeterm  = midTopCenterLeft.ComputeDeterminant();
            const T midTopCenterRightDeterm = midTopCenterRight.ComputeDeterminant();
            const T midTopRightDeterm       = midTopRight.ComputeDeterminant();

            const T midBotLeftDeterm        = midBotLeft.ComputeDeterminant();
            const T midBotCenterLeftDeterm  = midBotCenterLeft.ComputeDeterminant();
            const T midBotCenterRightDeterm = midBotCenterRight.ComputeDeterminant();
            const T midBotRightDeterm       = midBotRight.ComputeDeterminant();

            const T botLeftDeterm           = botLeft.ComputeDeterminant();
            const T botCenterLeftDeterm     = botCenterLeft.ComputeDeterminant();
            const T botCenterRightDeterm    = botCenterRight.ComputeDeterminant();
            const T botRightDeterm          = botRight.ComputeDeterminant();

            const Mat4<T> cofactors(
                 topLeftDeterm,     -topCenterLeftDeterm,     topCenterRightDeterm,    -topRightDeterm,
                -midTopLeftDeterm,   midTopCenterLeftDeterm, -midTopCenterRightDeterm,  midTopRightDeterm,
                 midBotLeftDeterm,  -midBotCenterLeftDeterm,  midBotCenterRightDeterm, -midBotRightDeterm,
                -botLeftDeterm,      botCenterLeftDeterm,    -botCenterRightDeterm,     botRightDeterm
            );

            return cofactors * invDeterminant;
        }
        // @formatter:on
        // clang-format on
    }

    template <typename T, U64 Width, U64 Height>
    constexpr Matrix<T, Width, Height>::Matrix(const Matrix<T, Width + 1, Height + 1>& mat) noexcept {
        U64 widthStride = 0;

        for (U64 heightIndex = 0; heightIndex < Height; ++heightIndex) {
            const U64 resultIndex = heightIndex * Width;

            for (U64 widthIndex = 0; widthIndex < Width; ++widthIndex) {
                const U64 finalIndex = resultIndex + widthIndex;

                m_Data[finalIndex] = mat[finalIndex + widthStride];
            }

            ++widthStride;
        }
    }

    template <typename T, U64 Width, U64 Height>
    constexpr Matrix<T, Width, Height>::Matrix(const Matrix<T, Width - 1, Height - 1>& mat) noexcept {
        U64 heightStride = 0;

        for (U64 widthIndex = 0; widthIndex < Width - 1; ++widthIndex) {
            const U64 finalWidthIndex = widthIndex * (Height - 1);

            for (U64 heightIndex = 0; heightIndex < Height - 1; ++heightIndex) {
                const U64 finalIndex = finalWidthIndex + heightIndex;

                m_Data[finalIndex + heightStride] = mat[finalIndex];
            }

            ++heightStride;
        }

        m_Data.back() = 1;
    }

    template <typename T, U64 Width, U64 Height>
    template <typename From, typename... Args, typename>
    constexpr Matrix<T, Width, Height>::Matrix(From&& val, Args&&... args) noexcept {
        static_assert(sizeof...(Args) == Width * Height - 1,
                      "[Error] A Fl::Matrix can't be constructed with more or less values than it can hold.");
        SetValues<0, 0>(std::forward<From>(val), std::forward<Args>(args)...);
    }

    template <typename T, U64 Width, U64 Height>
    constexpr U64 Matrix<T, Width, Height>::GetWidth() const noexcept {
        return Width;
    }

    template <typename T, U64 Width, U64 Height>
    constexpr U64 Matrix<T, Width, Height>::GetHeight() const noexcept {
        return Height;
    }

    template <typename T, U64 Width, U64 Height>
    constexpr const std::array<T, Width * Height>& Matrix<T, Width, Height>::GetData() const noexcept {
        return m_Data;
    }

    template <typename T, U64 Width, U64 Height>
    constexpr std::array<T, Width * Height>& Matrix<T, Width, Height>::GetData() noexcept {
        return m_Data;
    }

    template <typename T, U64 Width, U64 Height>
    constexpr const T* Matrix<T, Width, Height>::GetDataPtr() const noexcept {
        return m_Data.data();
    }

    template <typename T, U64 Width, U64 Height>
    constexpr T* Matrix<T, Width, Height>::GetDataPtr() noexcept {
        return m_Data.data();
    }

    template <typename T, U64 Width, U64 Height>
    constexpr const T& Matrix<
        T, Width, Height>::GetElement(const U64 widthIndex, const U64 heightIndex) const noexcept {
        return m_Data[widthIndex * Height + heightIndex];
    }

    template <typename T, U64 Width, U64 Height>
    constexpr T& Matrix<T, Width, Height>::GetElement(const U64 widthIndex, const U64 heightIndex) noexcept {
        return m_Data[widthIndex * Height + heightIndex];
    }

    template <typename T, U64 Width, U64 Height>
    constexpr Matrix<T, Width, Height> Matrix<T, Width, Height>::Identity() noexcept {
        static_assert(Width == Height, "[Error] Matrix must be square to be an identity matrix.");

        Matrix result;

        for (U64 diagIndex = 0; diagIndex < Width; ++diagIndex) {
            result[diagIndex * Width + diagIndex] = static_cast<T>(1);
        }

        return result;
    }

    template <typename T, U64 Width, U64 Height>
    template <typename... VectorTypes>
    constexpr Matrix<T, Width, Height> Matrix<T, Width, Height>::FromRows(VectorTypes&&... vecs) noexcept {
        static_assert(sizeof...(vecs) == Height,
                      "[Error] An Fl::Matrix cannot be constructed with more or less rows than it can hold.");

        Matrix result;
        result.SetRows(std::forward<VectorTypes>(vecs)...);
        return result;
    }

    template <typename T, U64 Width, U64 Height>
    template <typename... VectorTypes>
    constexpr Matrix<T, Width, Height> Matrix<T, Width, Height>::FromColumns(VectorTypes&&... vecs) noexcept {
        static_assert(sizeof...(vecs) == Width,
                      "[Error] An Fl::Matrix cannot be constructed with more or less columns than it can hold.");

        Matrix result;
        result.SetColumns(std::forward<VectorTypes>(vecs)...);
        return result;
    }

    template <typename T, U64 Width, U64 Height>
    constexpr Matrix<T, Height, Width> Matrix<T, Width, Height>::Transpose() const noexcept {
        Matrix<T, Height, Width> result;

        for (U64 widthIndex = 0; widthIndex < Width; widthIndex++) {
            const std::size_t finalWidthIndex = widthIndex * Height;

            for (U64 heightIndex = 0; heightIndex < Height; heightIndex++) {
                result[heightIndex * Width + widthIndex] = m_Data[finalWidthIndex + heightIndex];
            }
        }

        return result;
    }

    template <typename T, U64 Width, U64 Height>
    constexpr T Matrix<T, Width, Height>::ComputeDeterminant() const noexcept {
        static_assert(Width == Height, "[Error] Matrix must be a square one.");

        return Detail::ComputeMatrixDeterminant(*this);
    }

    template <typename T, U64 Width, U64 Height>
    constexpr Matrix<T, Width, Height> Matrix<T, Width, Height>::Inverse() const noexcept {
        static_assert(Width == Height, "[Error] Matrix must be a square one.");

        const T determinant = Detail::ComputeMatrixDeterminant(*this);

        if (determinant == 0) {
            return *this;
        }

        return Detail::ComputeMatrixInverse(*this, static_cast<T>(1) / determinant);
    }

    template <typename T, U64 Width, U64 Height>
    constexpr Vector<T, Width> Matrix<T, Width, Height>::GetRow(const U64 rowIndex) const noexcept {
        FlAssert(rowIndex < Height, "Given row index is out of bound.");

        Vector<T, Width> result;

        for (U64 valIndex = 0; valIndex < Width; valIndex++) {
            result[valIndex] = m_Data[rowIndex + valIndex * Height];
        }

        return result;
    }

    template <typename T, U64 Width, U64 Height>
    constexpr Vector<T, Height> Matrix<T, Width, Height>::GetColumn(const U64 columnIndex) const noexcept {
        FlAssert(columnIndex < Width, "Given column index is out of bound.");

        Vector<T, Height> result;

        const U64 columnBeginIndex = columnIndex * Height;

        for (U64 valIndex = 0; valIndex < Height; valIndex++) {
            result[valIndex] = m_Data[columnBeginIndex + valIndex];
        }

        return result;
    }

    template <typename T, U64 Width, U64 Height>
    constexpr bool Matrix<T, Width, Height>::StrictlyEquals(const Matrix& mat) const noexcept {
        return std::equal(m_Data.cbegin(), m_Data.cend(), mat.GetData().cbegin());
    }

    template <typename T, U64 Width, U64 Height>
    constexpr U64 Matrix<T, Width, Height>::Hash(U64 seed) const noexcept {
        std::hash<T> hasher{};

        for (const T& element : m_Data) {
            seed ^= hasher(element) + 0x9e3779b9 + (seed << 6u) + (seed >> 2u);
        }

        return seed;
    }

    template <typename T, U64 Width, U64 Height>
    constexpr Matrix<T, Width, Height> Matrix<T, Width, Height>::operator+(const Matrix& mat) const noexcept {
        Matrix result = *this;

        result += mat;

        return result;
    }

    template <typename T, U64 Width, U64 Height>
    constexpr Matrix<T, Width, Height> Matrix<T, Width, Height>::operator+(T val) const noexcept {
        Matrix result = *this;

        result += val;

        return result;
    }

    template <typename T, U64 Width, U64 Height>
    constexpr Matrix<T, Width, Height> Matrix<T, Width, Height>::operator-(const Matrix& mat) const noexcept {
        Matrix result = *this;

        result -= mat;

        return result;
    }

    template <typename T, U64 Width, U64 Height>
    constexpr Matrix<T, Width, Height> Matrix<T, Width, Height>::operator-(T val) const noexcept {
        Matrix result = *this;

        result -= val;

        return result;
    }

    template <typename T, U64 Width, U64 Height>
    constexpr Matrix<T, Width, Height> Matrix<T, Width, Height>::operator%(const Matrix& mat) const noexcept {
        Matrix result = *this;

        result %= mat;

        return result;
    }

    template <typename T, U64 Width, U64 Height>
    constexpr Matrix<T, Width, Height> Matrix<T, Width, Height>::operator*(T val) const noexcept {
        Matrix result = *this;

        result *= val;

        return result;
    }

    template <typename T, U64 Width, U64 Height>
    constexpr Matrix<T, Width, Height> Matrix<T, Width, Height>::operator/(const Matrix& mat) const noexcept {
        Matrix result = *this;

        result /= mat;

        return result;
    }

    template <typename T, U64 Width, U64 Height>
    constexpr Matrix<T, Width, Height> Matrix<T, Width, Height>::operator/(T val) const noexcept {
        Matrix result = *this;

        result /= val;

        return result;
    }

    template <typename T, U64 Width, U64 Height>
    constexpr Matrix<T, Width, Height>& Matrix<T, Width, Height>::operator+=(const Matrix& mat) noexcept {
        for (U64 i = 0; i < m_Data.size(); i++) {
            m_Data[i] += mat[i];
        }

        return *this;
    }

    template <typename T, U64 Width, U64 Height>
    constexpr Matrix<T, Width, Height>& Matrix<T, Width, Height>::operator+=(T val) noexcept {
        for (T& element : m_Data) {
            element += val;
        }

        return *this;
    }

    template <typename T, U64 Width, U64 Height>
    constexpr Matrix<T, Width, Height>& Matrix<T, Width, Height>::operator-=(const Matrix& mat) noexcept {
        for (U64 i = 0; i < m_Data.size(); i++) {
            m_Data[i] -= mat[i];
        }

        return *this;
    }

    template <typename T, U64 Width, U64 Height>
    constexpr Matrix<T, Width, Height>& Matrix<T, Width, Height>::operator-=(T val) noexcept {
        for (T& element : m_Data) {
            element -= val;
        }

        return *this;
    }

    template <typename T, U64 Width, U64 Height>
    constexpr Matrix<T, Width, Height>& Matrix<T, Width, Height>::operator%=(const Matrix& mat) noexcept {
        for (U64 i = 0; i < m_Data.size(); i++) {
            m_Data[i] *= mat[i];
        }

        return *this;
    }

    template <typename T, U64 Width, U64 Height>
    constexpr Matrix<T, Width, Height>& Matrix<T, Width, Height>::operator*=(T val) noexcept {
        for (T& element : m_Data) {
            element *= val;
        }

        return *this;
    }

    template <typename T, U64 Width, U64 Height>
    constexpr Matrix<T, Width, Height>& Matrix<T, Width, Height>::operator/=(const Matrix& mat) noexcept {
        for (U64 i = 0; i < m_Data.size(); i++) {
            m_Data[i] /= mat[i];
        }

        return *this;
    }

    template <typename T, U64 Width, U64 Height>
    constexpr Matrix<T, Width, Height>& Matrix<T, Width, Height>::operator/=(T val) noexcept {
        for (T& element : m_Data) {
            element /= val;
        }

        return *this;
    }

    template <typename T, U64 Width, U64 Height>
    constexpr Matrix<T, Width, Height>& Matrix<T, Width, Height>::operator*=(const Matrix& mat) noexcept {
        *this = *this * mat;
        return *this;
    }

    template <typename T, U64 Width, U64 Height>
    constexpr const T& Matrix<T, Width, Height>::operator[](const U64 index) const noexcept {
        return m_Data[index];
    }

    template <typename T, U64 Width, U64 Height>
    constexpr T& Matrix<T, Width, Height>::operator[](const U64 index) noexcept {
        return m_Data[index];
    }

    template <typename T, U64 Width, U64 Height>
    constexpr bool Matrix<T, Width, Height>::operator==(const Matrix& mat) const noexcept {
        if constexpr (std::is_floating_point_v<T>) {
            return FloatMath::AreNearlyEqual(*this, mat);
        } else {
            return StrictlyEquals(mat);
        }
    }

    template <typename T, U64 Width, U64 Height>
    constexpr bool Matrix<T, Width, Height>::operator!=(const Matrix& mat) const noexcept {
        return !(*this == mat);
    }

    template <typename T, U64 Width, U64 Height>
    std::ostream& operator<<(std::ostream& stream, const Matrix<T, Width, Height>& mat) {
        stream << '[';

        for (U64 heightIndex = 0; heightIndex < Height; heightIndex++) {
            if (heightIndex > 0) {
                stream << ' ';
            }

            stream << "[ ";

            for (U64 widthIndex = 0; widthIndex < Width; widthIndex++) {
                stream << mat.GetElement(widthIndex, heightIndex);

                if (widthIndex < Width - 1) {
                    stream << ", ";
                }
            }

            stream << " ]";

            if (heightIndex < Height - 1) {
                stream << '\n';
            }
        }

        stream << ']';

        return stream;
    }

    template <typename T, U64 Width, U64 Height>
    template <U64 WidthIndex, U64 HeightIndex, typename From, typename... Args>
    constexpr void Matrix<T, Width, Height>::SetValues(From&& val, Args&&... args) noexcept {
        static_assert(std::is_same_v<T, std::decay_t<From>>,
                      "[Error] An Fl::Matrix can only be constructed from values of the same type as its inner one.");

        m_Data[WidthIndex * Height + HeightIndex] = std::forward<From>(val);

        if constexpr (sizeof...(args) > 0) {
            constexpr U64 newWidthIndex = (WidthIndex + 1) % Width;
            SetValues<newWidthIndex, HeightIndex + (newWidthIndex == 0)>(std::forward<Args>(args)...);
        }
    }

    template <typename T, U64 Width, U64 Height>
    template <typename VectorType, typename... VectorTypes>
    constexpr void Matrix<T, Width, Height>::SetRows(VectorType&& vec, VectorTypes&&... vecs) noexcept {
        static_assert(std::is_same_v<std::decay_t<VectorType>, Vector<T, Width>>,
                      "[Error] Rows must all be vectors of the same type and size.");

        constexpr U64 firstIndex = Height - sizeof...(vecs) - 1;

        for (U64 widthIndex = 0; widthIndex < Width; ++widthIndex) {
            m_Data[firstIndex + widthIndex * Height] = std::forward<VectorType>(vec)[widthIndex];
        }

        if constexpr (sizeof...(vecs) > 0) {
            SetRows(std::forward<VectorTypes>(vecs)...);
        }
    }

    template <typename T, U64 Width, U64 Height>
    template <typename VectorType, typename... VectorTypes>
    constexpr void Matrix<T, Width, Height>::SetColumns(VectorType&& vec, VectorTypes&&... vecs) noexcept {
        static_assert(std::is_same_v<std::decay_t<VectorType>, Vector<T, Height>>,
                      "[Error] Columns must all be vectors of the same type and size.");

        constexpr U64 firstIndex = Height * (Width - sizeof...(vecs) - 1);

        for (U64 heightIndex = 0; heightIndex < Height; ++heightIndex) {
            m_Data[firstIndex + heightIndex] = std::forward<VectorType>(vec)[heightIndex];
        }

        if constexpr (sizeof...(vecs) > 0) {
            SetColumns(std::forward<VectorTypes>(vecs)...);
        }
    }

    template <typename T, U64 WidthLeft, U64 HeightLeft, U64 WidthRight, U64 HeightRight>
    constexpr Matrix<T, HeightLeft, WidthRight> operator*(const Matrix<T, WidthLeft, HeightLeft>& mat1,
                                                          const Matrix<T, WidthRight, HeightRight>& mat2) {
        static_assert(WidthLeft == HeightRight,
                      "[Error] The left-hand side matrix's width must be equal to the right-hand side matrix's height.")
            ;

        Matrix<T, HeightLeft, WidthRight> result;

        for (U64 widthIndex = 0; widthIndex < WidthRight; ++widthIndex) {
            const U64 finalWidthIndex = widthIndex * HeightLeft;

            for (U64 heightIndex = 0; heightIndex < HeightLeft; ++heightIndex) {
                T& val = result[finalWidthIndex + heightIndex];

                for (U64 stride = 0; stride < WidthLeft; ++stride) {
                    val += mat1.GetElement(stride, heightIndex) * mat2.GetElement(widthIndex, stride);
                }
            }
        }

        return result;
    }

    template <typename T, U64 Width, U64 Height>
    constexpr Vector<T, Height> operator*(const Matrix<T, Width, Height>& mat, const Vector<T, Width>& vec) {
        // This multiplication is made assuming the vector to be vertical.
        Vector<T, Height> result;

        for (U64 widthIndex = 0; widthIndex < Width; ++widthIndex) {
            const U64 finalWidthIndex = widthIndex * Height;

            for (U64 heightIndex = 0; heightIndex < Height; ++heightIndex) {
                result[heightIndex] += mat[finalWidthIndex + heightIndex] * vec[widthIndex];
            }
        }

        return result;
    }

    template <typename T, U64 Width, U64 Height>
    constexpr Vector<T, Width> operator*(const Vector<T, Height>& vec, const Matrix<T, Width, Height>& mat) {
        // This multiplication is made assuming the vector to be horizontal.
        Vector<T, Width> result;

        for (U64 widthIndex = 0; widthIndex < Width; ++widthIndex) {
            const U64 finalWidthIndex = widthIndex * Height;

            for (U64 heightIndex = 0; heightIndex < Height; ++heightIndex) {
                result[widthIndex] += vec[heightIndex] * mat[finalWidthIndex + heightIndex];
            }
        }

        return result;
    }
}

// clang-format off
// @formatter:off
template <typename T, Fl::U64 Width, Fl::U64 Height>
constexpr std::size_t std::hash<Fl::Matrix<T, Width, Height>>::operator()(const Fl::Matrix<T, Width, Height>& mat) const noexcept {
    return mat.Hash();
}

template <typename T, Fl::U64 Width, Fl::U64 Height>
constexpr std::size_t std::equal_to<Fl::Matrix<T, Width, Height>>::operator()(const Fl::Matrix<T, Width, Height>& mat1,
                                                                              const Fl::Matrix<T, Width, Height>& mat2) const noexcept {
    return mat1.StrictlyEquals(mat2);
}

template <typename T, Fl::U64 Width, Fl::U64 Height>
constexpr bool std::less<Fl::Matrix<T, Width, Height>>::operator()(const Fl::Matrix<T, Width, Height>& mat1,
                                                                   const Fl::Matrix<T, Width, Height>& mat2) const noexcept{
    for (std::size_t i = 0; i < Width * Height; ++i) {
        if (mat1[i] == mat2[i]) {
            continue;
        }

        return (mat1[i] < mat2[i]);
    }

    return false;
}

// clang-format on
// @formatter:on
