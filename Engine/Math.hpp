#pragma once

#include "Utils.hpp"
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
            this->arr_[0] = x;
            this->arr_[1] = y;
        }

        Vector2() noexcept : Vector2(0, 0) {}
        Vector2(const Vector2<T>& vec) noexcept : Vector2(vec.arr_[0], vec.arr_[1]) {}
        Vector2(Vector2<T>&& vec) noexcept : Vector2(vec.arr_[0], vec.arr_[1]) {}

        ~Vector2() noexcept = default;

        Vector2<T>& operator=(const Vector2<T>& other) = default;
        Vector2<T>& operator=(Vector2<T>&& other) = default;

        T& operator[](usize index) {
            throw_if<index_error>(index <= 2, index_error {"Vector2 index is out of range!"});
            return this->arr_[index];
        }

        inline T& X() noexcept {
            return this->arr_[0];
        }

        inline T& Y() noexcept {
            return this->arr_[1];
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
            this->arr_[0] = x;
            this->arr_[1] = y;
            this->arr_[2] = z;
        }

        Vector3() noexcept : Vector3(0, 0, 0) {}
        Vector3(const Vector3<T>& vec) noexcept : Vector3(vec.arr_[0], vec.arr_[1], vec.arr_[2]) {}
        Vector3(Vector3<T>&& vec) noexcept : Vector3(vec.arr_[0], vec.arr_[1], vec.arr_[2]) {}

        ~Vector3() noexcept = default;

        Vector3<T>& operator=(const Vector3<T>& other) = default;
        Vector3<T>& operator=(Vector3<T>&& other) = default;

        T& operator[](usize index) {
            throw_if<index_error>(index <= 3, index_error {"Vector3 index is out of range!"});
            return this->arr_[index];
        }

        inline T& X() noexcept {
            return this->arr_[0];
        }

        inline T& Y() noexcept {
            return this->arr_[1];
        }

        inline T& Z() noexcept {
            return this->arr_[2];
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
            this->arr_[0] = x;
            this->arr_[1] = y;
            this->arr_[2] = z;
            this->arr_[3] = w;
        }

        Vector4() noexcept : Vector4(0, 0) {}
        Vector4(const Vector4<T>& vec) noexcept : Vector4(vec.arr_[0], vec.arr_[1], vec.arr_[2], vec.arr_[3]) {}
        Vector4(Vector4<T>&& vec) noexcept : Vector4(vec.arr_[0], vec.arr_[1], vec.arr_[2], vec.arr_[3]) {}

        ~Vector4() noexcept = default;

        Vector4<T>& operator=(const Vector4<T>& other) = default;
        Vector4<T>& operator=(Vector4<T>&& other) = default;

        inline T& operator[](usize index) {
            throw_if<index_error>(index <= 4, index_error {"Vector4 index is out of range!"});
            return this->arr_[index];
        }

        inline T& X() noexcept {
            return this->arr_[0];
        }

        inline T& Y() noexcept {
            return this->arr_[1];
        }

        inline T& Z() noexcept {
            return this->arr_[2];
        }

        inline T& W() noexcept {
            return this->arr_[3];
        }

        private:
        T arr_[4];
    };

    template <typename T = real32>
    class Matrix2 {
        private:
        T arr_[2][2];
    };

    template <typename T = real32>
    class Matrix3 {
        private:
        T arr_[3][3];
    };

    template <typename T = real32>
    class Matrix4 {
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
