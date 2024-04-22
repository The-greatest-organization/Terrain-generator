#pragma once

#include "Core/Utils.hpp"
#include <cmath>

namespace tiny3d {
    const real32 PI32 = 3.14159265359f;
    const real64 PI64 = 3.14159265359;

    template <typename T = real32>
    class Vector2 {
        public:
        using ValueType = T;

        Vector2(T x, T y) noexcept {
            static_assert(std::is_arithmetic_v<T>);
            arr_[0] = x;
            arr_[1] = y;
        }

        Vector2() noexcept : Vector2(0, 0) {}
        Vector2(const Vector2<T>& vec) noexcept : Vector2(vec.arr_[0], vec.arr_[1]) {}
        Vector2(Vector2<T>&& vec) noexcept : Vector2(vec.arr_[0], vec.arr_[1]) {}

        ~Vector2() noexcept = default;

        Vector2<T>& operator=(const Vector2<T>& other) = default;
        Vector2<T>& operator=(Vector2<T>&& other) = default;

        inline T& operator[](usize index) {
            assert(index <= 2);
            return arr_[index];
        }

        inline T operator[](usize index) const {
            assert(index <= 2);
            return arr_[index];
        }

        inline T& x() noexcept {
            return arr_[0];
        }

        inline T& y() noexcept {
            return arr_[1];
        }

        private:
        T arr_[2];
    };

    template <typename T = real32>
    class Vector3 {
        public:
        using ValueType = T;

        Vector3(T x, T y, T z) {
            static_assert(std::is_arithmetic_v<T>);
            arr_[0] = x;
            arr_[1] = y;
            arr_[2] = z;
        }

        Vector3() noexcept : Vector3(0, 0, 0) {}
        Vector3(const Vector3<T>& vec) noexcept : Vector3(vec.arr_[0], vec.arr_[1], vec.arr_[2]) {}
        Vector3(Vector3<T>&& vec) noexcept : Vector3(vec.arr_[0], vec.arr_[1], vec.arr_[2]) {}

        ~Vector3() noexcept = default;

        Vector3<T>& operator=(const Vector3<T>& other) = default;
        Vector3<T>& operator=(Vector3<T>&& other) = default;

        inline T& operator[](usize index) {
            assert(index <= 3);
            return arr_[index];
        }

        inline T operator[](usize index) const {
            assert(index <= 3);
            return arr_[index];
        }

        inline T& x() noexcept {
            return arr_[0];
        }

        inline T& y() noexcept {
            return arr_[1];
        }

        inline T& z() noexcept {
            return arr_[2];
        }

        private:
        T arr_[3];
    };

    template <typename T = real32>
    class Vector4 {
        public:
        using ValueType = T;

        Vector4(T x, T y, T z, T w) noexcept {
            static_assert(std::is_arithmetic_v<T>);
            arr_[0] = x;
            arr_[1] = y;
            arr_[2] = z;
            arr_[3] = w;
        }

        Vector4() noexcept : Vector4(0, 0) {}
        Vector4(const Vector4<T>& vec) noexcept : Vector4(vec.arr_[0], vec.arr_[1], vec.arr_[2], vec.arr_[3]) {}
        Vector4(Vector4<T>&& vec) noexcept : Vector4(vec.arr_[0], vec.arr_[1], vec.arr_[2], vec.arr_[3]) {}

        ~Vector4() noexcept = default;

        Vector4<T>& operator=(const Vector4<T>& other) = default;
        Vector4<T>& operator=(Vector4<T>&& other) = default;

        inline T& operator[](usize index) {
            assert(index <= 4);
            return arr_[index];
        }

        inline T operator[](usize index) const {
            assert(index <= 4);
            return arr_[index];
        }

        inline T& x() noexcept {
            return arr_[0];
        }

        inline T& y() noexcept {
            return arr_[1];
        }

        inline T& z() noexcept {
            return arr_[2];
        }

        inline T& w() noexcept {
            return arr_[3];
        }

        private:
        T arr_[4];
    };

    template <typename T = real32>
    class Matrix2 {
        public:
        using ValueType = T;

        Matrix2(T* ptr) {
            static_assert(std::is_arithmetic_v<T>);
            assert(ptr != nullptr);

            arr_[0][0] = ptr[0][0];
            arr_[0][1] = ptr[0][1];

            arr_[1][0] = ptr[1][0];
            arr_[1][1] = ptr[1][1];
        }

        Matrix2(T x, T y) noexcept {
            static_assert(std::is_arithmetic_v<T>);

            arr_[0][0] = x;
            arr_[0][1] = 0;

            arr_[1][0] = 0;
            arr_[1][1] = y;
        }

        private:
        T arr_[2][2];
    };

    template <typename T = real32>
    class Matrix3 {
        public:
        using ValueType = T;

        Matrix3(T* ptr) {
            static_assert(std::is_arithmetic_v<T>);
        }

        Matrix3(T x, T y, T z) noexcept {
            static_assert(std::is_arithmetic_v<T>);

            arr_[0][0] = x;
            arr_[0][1] = 0;
            arr_[0][2] = 0;

            arr_[1][0] = 0;
            arr_[1][1] = y;
            arr_[1][2] = 0;

            arr_[2][0] = 0;
            arr_[2][1] = 0;
            arr_[2][2] = z;
        }

        private:
        T arr_[3][3];
    };

    template <typename T = real32>
    class Matrix4 {
        public:
        using ValueType = T;

        private:
        T arr_[4][4];
    };

    using V2 = Vector2<>;
    using V3 = Vector3<>;
    using V4 = Vector4<>;

    using Mat2 = Matrix2<>;
    using Mat3 = Matrix3<>;
    using Mat4 = Matrix4<>;
}
