#pragma once

#include "Core/Utils.hpp"
#include "Core/Memory.hpp"
#include "Math.hpp"

namespace tiny3d {

    // Data = the body of component. Each component is based on some data
    // Priority = the importance of component (for dispatcher)

    /// @todo -- (BIG ONE) -- Rewrite component system with resource manager which works with raw memory

    template <typename T>
    class ObjectComponent {
        public:
        /// @todo: Realize all ctors I need (rule of 5)

        shared_ptr<T> Data() {
            return this->data_;
        }

        protected:
        void set_component(shared_ptr<T> data) {
            data_.reset();
            data_ = data;
        }

        void set_component(raw_ptr<T> data) {
            assert(data != nullptr);
            this->data_.reset();
            this->data_ = make_shared<T>(data);
        }

        void set_component(const T& data) {
            static_assert(std::is_copy_constructible_v<T>);
            this->data_.reset();
            this->data_ = make_shared<T>(data);
        }

        void set_component(T&& data) {
            static_assert(std::is_move_constructible_v<T>);
            this->data_.reset();
            this->data_ = make_shared<T>(data);
        }

        shared_ptr<T> data_ {nullptr};
    };

    // All non-primitive data types should be packed into components

    // Transform components
    /// @todo add method 'calculate_matrix' to all transform components

    class Translation2D : public ObjectComponent<V2> {};
    class Rotation2D : public ObjectComponent<V2> {};
    class Scale2D : public ObjectComponent<V2> {};

    class Translation3D : public ObjectComponent<V3> {};
    class Rotation3D : public ObjectComponent<V3> {};
    class Scale3D : public ObjectComponent<V3> {};

    /// @todo implement graphics components

    // Graphics components will be looking someting like this
    // class RenderComponent : public ObjectComponent<RenderData> {};

    // Object = just collection of components (which are also presents data) + some primitive variables

    // Renderer = RenderTarget (texture or screen) + Camera
    // On scene there're many renderers
}