#pragma once

#include "Utils.hpp"
#include <cmath>

namespace tiny3d {
    template <typename T = real32>
    class Vector2 {
        public:
        using ValueType = T;

        Vector2(T x, T y) noexcept {
            static_assert(std::is_arithmetic_v<T>);
            this->data_ = {x, y};
        }

        Vector2() noexcept : Vector2(0, 0) {}
        Vector2(const Vector2<T>& ref) noexcept : Vector2(ref.arr_[0], ref.arr_[1]) {}
        Vector2(Vector2<T>&& ref) noexcept : Vector2(ref.arr_[0], ref.arr_[1]) {}

        ~Vector2() noexcept = default;

        Vector2<T>& operator=(const Vector2<T>& other) = default;
        Vector2<T>& operator=(Vector2<T>&& other) = default;

        T& operator[](usize index) {
            throw_if<out_of_range>(index <= 2, out_of_range {"Vector2 index is out of range!"});
            return this->arr_[index];
        }

        inline T& X() noexcept {
            return this->arr_[0];
        }

        inline T& Y() noexcept {
            return this->arr_[1];
        }

        private:
        union {
            struct {
                T x, y;
            } data_;
            T arr_[2];
        };
    };

    template <typename T = real32>
    class Vector3 {
        public:
        using ValueType = T;

        Vector3(T x, T y, T z) {
            static_assert(std::is_arithmetic_v<T>);
            this->data_ = {x, y, z};
        }

        Vector3() noexcept : Vector3(0, 0, 0) {}
        Vector3(const Vector3<T>& ref) noexcept : Vector3(ref.arr_[0], ref.arr_[1], ref.arr_[2]) {}
        Vector3(Vector3<T>&& ref) noexcept : Vector3(ref.arr_[0], ref.arr_[1], ref.arr_[2]) {}

        ~Vector3() noexcept = default;

        Vector3<T>& operator=(const Vector3<T>& other) = default;
        Vector3<T>& operator=(Vector3<T>&& other) = default;

        T& operator[](usize index) {
            throw_if<out_of_range>(index <= 3, out_of_range {"Vector3 index is out of range!"});
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
        union {
            struct {
                T x, y, z;
            } data_;
            T arr_[3];
        };
    };
    
    template <typename T = real32>
    class Vector4 {
        public:
        using ValueType = T;

        Vector4(T x, T y, T z, T w) noexcept {
            static_assert(std::is_arithmetic_v<T>);
            this->data_ = {x, y, z, w};
        }
        
        Vector4() noexcept : Vector4(0, 0) {}
        Vector4(const Vector4<T>& ref) noexcept : Vector4(ref.arr_[0], ref.arr_[1], ref.arr_[2], ref.arr_[3]) {}
        Vector4(Vector4<T>&& ref) noexcept : Vector4(ref.arr_[0], ref.arr_[1], ref.arr_[2], ref.arr_[3]) {}

        ~Vector4() noexcept = default;

        Vector4<T>& operator=(const Vector4<T>& other) = default;
        Vector4<T>& operator=(Vector4<T>&& other) = default;

        inline T& operator[](usize index) {
            throw_if<out_of_range>(index <= 4, out_of_range {"Vector4 index is out of range!"});
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
        union {
            struct {
                T x, y, z, w;
            } data_;
            T arr_[4];
        };
    };
}
