// Copyright (C) 2025 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#ifndef FL_MATH_MATRIX_HPP
#define FL_MATH_MATRIX_HPP

#include <FlashlightEngine/Prerequisites.hpp>

#include <array>
#include <ostream>

namespace Fl {
    template <typename T, U64 Width, U64 Height>
    class Matrix;

    template <typename T, U64 Size>
    class Vector;

    template <typename T, U64 Width, U64 Height>
    std::ostream& operator<<(std::ostream& stream, const Matrix<T, Width, Height>& mat);

    /**
     * @brief Matrix class, representing a mathematical matrix, with generic type and size.
     * @tparam T Type of the matrix's data.
     * @tparam Width Matrix's width.
     * @tparam Height Matrix's height;
     */
    template <typename T, U64 Width, U64 Height>
    class Matrix {
    public:
        constexpr Matrix() noexcept = default;
        constexpr explicit Matrix(const Matrix<T, Width + 1, Height + 1>& mat) noexcept;
        constexpr explicit Matrix(const Matrix<T, Width - 1, Height - 1>& mat) noexcept;
        /**
         * @brief Constructs a matrix from the given values.
         * @note All values must be of the same type as the matrix's inner one, and there must be exactly as many values
         *       as the matrix can hold (width * height).
         * @tparam From Type of the first value. Must be the same as T.
         * @tparam Args Type of the other values. Must be the same as T.
         * @param val First value.
         * @param args other values.
         */
        template <typename From, typename... Args, typename = std::enable_if_t<std::is_same_v<std::decay_t<From>, T>>>
        constexpr explicit Matrix(From&& val, Args&&... args) noexcept;
        constexpr Matrix(const Matrix&) noexcept = default;
        constexpr Matrix(Matrix&&) noexcept = default;

        constexpr ~Matrix() noexcept = default;

        [[nodiscard]] constexpr U64 GetWidth() const noexcept;
        [[nodiscard]] constexpr U64 GetHeight() const noexcept;
        constexpr const std::array<T, Width * Height>& GetData() const noexcept;
        constexpr std::array<T, Width * Height>& GetData() noexcept;
        constexpr const T* GetDataPtr() const noexcept;
        constexpr T* GetDataPtr() noexcept;
        /**
         * @brief Gets an element given its width & height indices.
         * @param widthIndex Element's width index.
         * @param heightIndex Element's height index.
         * @return A constant reference to the element.
         */
        constexpr const T& GetElement(U64 widthIndex, U64 heightIndex) const noexcept;
        /**
         * @brief Gets an element given its width & height indices.
         * @param widthIndex Element's width index.
         * @param heightIndex Element's height index.
         * @return A reference to the element.
         */
        constexpr T& GetElement(U64 widthIndex, U64 heightIndex) noexcept;

        /**
         * @brief Creates an identity matrix; needs to be called with a square matrix type.
         * @return Identity matrix.
         */
        static constexpr Matrix Identity() noexcept;
        /**
         * @brief Constructs a matrix from the given row vectors.
         * @note All vectors must be of the same inner type as the matrix's, and must have a size equal to the matrix's
         *       width.
         * @tparam VectorTypes Types of the vectors to construct the matrix with.
         * @param vecs Row vectors to construct the matrix with.
         * @return A matrix with the given rows.
         */
        template <typename... VectorTypes>
        static constexpr Matrix FromRows(VectorTypes&&... vecs) noexcept;
        /**
         * @brief Constructs a matrix from the given column vectors.
         * @note All vectors must be of the same inner type as the matrix's, and must have a size equal to the matrix's
         *       height.
         * @tparam VectorTypes Types of the vectors to construct the matrix with.
         * @param vecs Column vectors to construct the matrix with.
         * @return A matrix with the given columns.
         */
        template <typename... VectorTypes>
        static constexpr Matrix FromColumns(VectorTypes&&... vecs) noexcept;

        /**
         * @brief Transposes the current matrix.
         * @return Transposed matrix.
         */
        constexpr Matrix<T, Height, Width> Transpose() const noexcept;
        /**
         * @brief Computes the matrix's determinant.
         * @return Matrix's determinant.
         */
        constexpr T ComputeDeterminant() const noexcept;
        /**
         * @brief Computes the matrix's inverse.
         * @return Matrix's inverse.
         */
        constexpr Matrix Inverse() const noexcept;
        /**
         * @brief Gets the values of the row at the given index.
         * @param rowIndex Index of the row to retrieve.
         * @return Vector containing the values of the requested row.
         */
        constexpr Vector<T, Width> GetRow(U64 rowIndex) const noexcept;
        /**
         * @brief Gets the values of the column at the given index.
         * @param columnIndex Index of the column to retrieve.
         * @return Vector containing the values of the requested column.
         */
        constexpr Vector<T, Height> GetColumn(U64 columnIndex) const noexcept;
        /**
         * @brief Checks for strict equality between the current matrix & the given one.
         * @param mat Matrix to compare the current one to.
         * @return Whether both matrices are strictly equal to each other or not.
         */
        [[nodiscard]] constexpr bool StrictlyEquals(const Matrix& mat) const noexcept;
        /**
         * @brief Computes the unique hash of the current matrix.
         * @param seed Value to use as a hash seed.
         * @return Matrix's hash.
         */
        [[nodiscard]] constexpr U64 Hash(U64 seed = 0) const noexcept;

        /**
         * @brief Default copy assignment operator.
         * @return Reference to the copied matrix.
         */
        constexpr Matrix& operator=(const Matrix&) noexcept = default;
        /**
         * @brief Default move assignment operator.
         * @return Reference to the moved matrix.
         */
        constexpr Matrix& operator=(Matrix&&) noexcept = default;
        /**
         * @brief Element-wise matrix-matrix addition operator.
         * @param mat Matrix to be added.
         * @return Result of the summed matrices.
         */
        constexpr Matrix operator+(const Matrix& mat) const noexcept;
        /**
         * @brief Element-wise matrix-value addition operator.
         * @param val Value to be added.
         * @return Result of the matrix summed with the value.
         */
        constexpr Matrix operator+(T val) const noexcept;
        /**
         * @brief Element-wise matrix-matrix subtraction operator.
         * @param mat Matrix to be subtracted.
         * @return Result of the subtracted matrices.
         */
        constexpr Matrix operator-(const Matrix& mat) const noexcept;
        /**
         * @brief Element-wise matrix-value subtraction operator.
         * @param val Value to be subtracted.
         * @return Result of the matrix subtracted by the value.
         */
        constexpr Matrix operator-(T val) const noexcept;
        /**
         * @brief Element-wise matrix-matrix multiplication operator.
         * @param mat Matrix to be multiplied by.
         * @return Result of the multiplied matrices.
         */
        constexpr Matrix operator%(const Matrix& mat) const noexcept;
        /**
         * @brief Element-wise matrix-value multiplication operator.
         * @param val Value to be multiplied by.
         * @return Result of the matrix multiplied by the value.
         */
        constexpr Matrix operator*(T val) const noexcept;
        /**
         * @brief Element-wise matrix-matrix division operator.
         * @param mat Matrix to be divided by.
         * @return Result of the divided matrices.
         */
        constexpr Matrix operator/(const Matrix& mat) const noexcept;
        /**
         * @brief Element-wise matrix-value division operator.
         * @param val Value to be divided by.
         * @return Result of the matrix divided by the value.
         */
        constexpr Matrix operator/(T val) const noexcept;
        /**
         * @brief Element-wise matrix-matrix addition assignment operator.
         * @param mat Matrix to be added.
         * @return Reference to the modified original matrix.
         */
        constexpr Matrix& operator+=(const Matrix& mat) noexcept;
        /**
         * @brief Element-wise matrix-value addition assignment operator.
         * @param val Value to be added.
         * @return Reference to the modified original matrix.
         */
        constexpr Matrix& operator+=(T val) noexcept;
        /**
         * @brief Element-wise matrix-matrix subtraction assignment operator.
         * @param mat Matrix to be subtracted.
         * @return Reference to the modified original matrix.
         */
        constexpr Matrix& operator-=(const Matrix& mat) noexcept;
        /**
         * @brief Element-wise matrix-value subtraction assignment operator.
         * @param val Value to be subtracted.
         * @return Reference to the modified original matrix.
         */
        constexpr Matrix& operator-=(T val) noexcept;
        /**
         * @brief Element-wise matrix-matrix multiplication assignment operator.
         * @param mat Matrix to be multiplied by.
         * @return Reference to the modified original matrix.
         */
        constexpr Matrix& operator%=(const Matrix& mat) noexcept;
        /**
         * @brief Element-wise matrix-value multiplication assignment operator.
         * @param val Value to be multiplied by.
         * @return Reference to the modified original matrix.
         */
        constexpr Matrix& operator*=(T val) noexcept;
        /**
         * @brief Element-wise matrix-matrix division assignment operator.
         * @param mat Matrix to be divided by.
         * @return Reference to the modified original matrix.
         */
        constexpr Matrix& operator/=(const Matrix& mat) noexcept;
        /**
         * @brief Element-wise matrix-value division assignment operator.
         * @param val Value to be divided by.
         * @return Reference to the modified original matrix.
         */
        constexpr Matrix& operator/=(T val) noexcept;
        /**
         * @brief Matrix-matrix multiplication assignment operator.
         * @param mat Matrix to be multiplied by.
         * @return Reference to the modified original matrix.
         */
        constexpr Matrix& operator*=(const Matrix& mat) noexcept;
        /**
         * @brief Element fetching operator with a single index.
         * @warning Fl::Matrix uses a column-major layout, therefore this operator should be used with caution. For
         *          example, for a 3x3 matrix, the indices are laid out as follows:
         *
         *          0 3 6
         *          1 4 7
         *          2 5 8
         *
         *          Prefer using @ref GetElement for better readability.
         * @param index Element's index.
         * @return Constant reference to the element.
         */
        constexpr const T& operator[](U64 index) const noexcept;
        /**
         * @brief Element fetching operator with a single index.
         * @warning Fl::Matrix uses a column-major layout, therefore this operator should be used with caution. For
         *          example, for a 3x3 matrix, the indices are laid out as follows:
         *
         *          0 3 6
         *          1 4 7
         *          2 5 8
         *
         *          Prefer using @ref GetElement for better readability.
         * @param index Element's index.
         * @return Reference to the element.
         */
        constexpr T& operator[](U64 index) noexcept;
        /**
         * @brief Matrix equality comparison operator. Uses a near-equality algorithm for floating-point values to take
         *        floating point errors into account.
         * @param mat Matrix to be compared with.
         * @return Whether both matrices are nearly equal or not.
         */
        constexpr bool operator==(const Matrix& mat) const noexcept;
        /**
         * @brief Matrix inequality comparison operator. Uses a near-equality algorithm for floating-point values to
         *        take floating point errors into account.
         * @param mat Matrix to be compared with.
         * @return Whether both matrices are unequal or not.
         */
        constexpr bool operator!=(const Matrix& mat) const noexcept;
        /**
         * @brief Output stream operator.
         * @param stream Stream to output into.
         * @param mat Matrix to be output.
         * @return Reference to the modified stream.
         */
        friend std::ostream& operator<<<>(std::ostream& stream, const Matrix& mat);

    private:
        template <U64 WidthIndex, U64 HeightIndex, typename From, typename... Args>
        constexpr void SetValues(From&& val, Args&&... args) noexcept;

        template <typename VectorType, typename... VectorTypes>
        constexpr void SetRows(VectorType&& vec, VectorTypes&&... vecs) noexcept;

        template <typename VectorType, typename... VectorTypes>
        constexpr void SetColumns(VectorType&& vec, VectorTypes&&... vecs) noexcept;

        std::array<T, Width * Height> m_Data{};
    };

    /**
     * @brief Matrix-matrix multiplication operator.
     * @tparam T Type of the matrices' data.
     * @tparam WidthLeft Type of the matrices' data.
     * @tparam HeightLeft Height of the left-hand side matrix & width of the resulting one.
     * @tparam WidthRight Width of the right-hand side matrix & height of the resulting one.
     * @tparam HeightRight Height of the right-hand side matrix.
     * @param mat1 Left-hand side matrix.
     * @param mat2 Right-hand side matrix.
     * @return Result of the multiplied matrices.
     */
    template <typename T, U64 WidthLeft, U64 HeightLeft, U64 WidthRight, U64 HeightRight>
    constexpr Matrix<T, HeightLeft, WidthRight> operator*(const Matrix<T, WidthLeft, HeightLeft>& mat1,
                                                          const Matrix<T, WidthRight, HeightRight>& mat2);

    /**
     * @brief Matrix-vector multiplication operator (assumes the vector to be vertical).
     * @tparam T Type of the matrix's and vector's data.
     * @tparam Width Width of the matrix and size of the vector.
     * @tparam Height Height of the matrix and size of the resulting vector.
     * @param mat Left-hand side matrix.
     * @param vec Right-hand side vector.
     * @return Result of the matrix-vector multiplication.
     */
    template <typename T, U64 Width, U64 Height>
    constexpr Vector<T, Height> operator*(const Matrix<T, Width, Height>& mat, const Vector<T, Width>& vec);

    /**
     * @brief Matrix-vector multiplication operator (assumes the vector to be vertical).
     * @tparam T Type of the matrix's and vector's data.
     * @tparam Width Width of the matrix and size of the resulting vector.
     * @tparam Height Height of the matrix and size of the vector.
     * @param vec Left-hand side vector.
     * @param mat Right-hand side matrix.
     * @return Result of the matrix-vector multiplication.
     */
    template <typename T, U64 Width, U64 Height>
    constexpr Vector<T, Width> operator*(const Vector<T, Height>& vec, const Matrix<T, Width, Height>& mat);

    // Aliases

    template <typename T>
    using Mat2 = Matrix<T, 2, 2>;

    template <typename T>
    using Mat3 = Matrix<T, 3, 3>;

    template <typename T>
    using Mat4 = Matrix<T, 4, 4>;

    using Mat2b = Mat2<U8>;
    using Mat3b = Mat3<U8>;
    using Mat4b = Mat4<U8>;

    using Mat2i = Mat2<I32>;
    using Mat3i = Mat3<I32>;
    using Mat4i = Mat4<I32>;

    using Mat2u = Mat2<U32>;
    using Mat3u = Mat3<U32>;
    using Mat4u = Mat4<U32>;

    using Mat2f = Mat2<F32>;
    using Mat3f = Mat3<F32>;
    using Mat4f = Mat4<F32>;

    using Mat2d = Mat2<F64>;
    using Mat3d = Mat3<F64>;
    using Mat4d = Mat4<F64>;
}

/**
 * @brief Specialization of std::hash for @ref Fl::Matrix.
 * @tparam T Type of the matrix's elements.
 * @tparam Width Width of the matrix.
 * @tparam Height Height of the matrix.
 */
template <typename T, Fl::U64 Width, Fl::U64 Height>
struct std::hash<Fl::Matrix<T, Width, Height>> {
    /**
     * @brief Computes the hash of the given matrix.
     * @param mat Matrix to compute the hash for.
     * @return Matrix's hash value.
     */
    constexpr std::size_t operator()(const Fl::Matrix<T, Width, Height>& mat) const noexcept;
};

/**
 * @brief Specialization of std::equal_to for @ref Fl::Matrix. This performs a strict equality check.
 * @tparam T Type of the matrices' elements.
 * @tparam Width Width of the matrices.
 * @tparam Height Height of the matrices.
 */
template <typename T, Fl::U64 Width, Fl::U64 Height>
struct std::equal_to<Fl::Matrix<T, Width, Height>> {
    /**
     * @brief Checks that the two given matrices are strictly equal to each other.
     * @param mat1 First matrix to compare.
     * @param mat2 Second matrix to compare.
     * @return Whether both matrices are equal to each other or not.
     */
    constexpr std::size_t operator()(const Fl::Matrix<T, Width, Height>& mat1,
                                     const Fl::Matrix<T, Width, Height>& mat2) const noexcept;
};

/**
 * @brief Specialization of std::less for @ref Fl::Matrix.
 * @tparam T Type of the matrices' elements.
 * @tparam Width Width of the matrices.
 * @tparam Height Height of the matrices.
 */
template <typename T, Fl::U64 Width, Fl::U64 Height>
struct std::less<Fl::Matrix<T, Width, Height>> {
    /**
     * @brief Checks that the first given matrix is strictly less than the other.
     * @param mat1 First matrix to compare.
     * @param mat2 Second matrix to compare.
     * @return Whether the first matrix is strictly less than the other or not.
     */
    constexpr bool operator()(const Fl::Matrix<T, Width, Height>& mat1,
                              const Fl::Matrix<T, Width, Height>& mat2) const noexcept;
};

#include <FlashlightEngine/Math/Matrix.inl>

#endif // FL_MATH_MATRIX_HPP
