// by andrew.la
#pragma once

#include "Core/Utils.hpp"
#include "Core/Memory.hpp"
#include "Core/Math.hpp"

#include <unordered_map>

namespace tiny3d {
    template <typename T> concept ComponentType = std::movable<T> && std::copyable<T> && std::destructible<T>;

    /// @todo fill some general fields
    struct ObjectComponent {};

    // Data = the body of component. Each component is based on some data

    template <ComponentType T>
    class ObjectComponentT : public ObjectComponent {
        public:
        using Type = T;

        ObjectComponentT() = delete;
        ObjectComponentT(const ObjectComponentT& component) : data_(component.data_) {}
        ObjectComponentT(ObjectComponentT&& component) : data_(std::move(component.data_)) {}

        template <typename... Args>
        ObjectComponentT(Args&&... args) : data_(std::forward<Args>(args)...) {}

        template <typename U>
        ObjectComponentT(U&& ref) {
            static_assert(std::is_same_v<typename std::remove_reference_t<U>, T>);
            data_ = T{std::forward<U>(ref)};
        }

        virtual ~ObjectComponentT() = default;

        inline T& Get() {
            return data_;
        }

        inline T* Ptr() {
            return &data_;
        }

        protected:
        T data_;
    };

    class Translation2D : public ObjectComponentT<glm::vec2> {
        public:
        using ObjectComponentT<glm::vec2>::ObjectComponentT;
        glm::mat3 CalculateMatrix() const;
    };

    class Rotation2D : public ObjectComponentT<glm::vec2> {
        public:
        using ObjectComponentT<glm::vec2>::ObjectComponentT;
        glm::mat3 CalculateMatrix() const;
    };

    class Scale2D : public ObjectComponentT<glm::vec2> {
        public:
        using ObjectComponentT<glm::vec2>::ObjectComponentT;
        glm::mat3 CalculateMatrix() const;
    };

    class Translation3D : public ObjectComponentT<glm::vec4> {
        public:
        using ObjectComponentT<glm::vec4>::ObjectComponentT;
        glm::mat3 CalculateMatrix() const;
    };

    class Rotation3D : public ObjectComponentT<glm::vec4> {
        public:
        using ObjectComponentT<glm::vec4>::ObjectComponentT;
        glm::mat3 CalculateMatrix() const;
    };

    class Scale3D : public ObjectComponentT<glm::vec4> {
        public:
        using ObjectComponentT<glm::vec4>::ObjectComponentT;
        glm::mat3 CalculateMatrix() const;
    };

    /// @todo implement graphics components

    // Graphics components will be looking someting like this
    // class RenderComponent : public ObjectComponent<RenderData> {};

    // Object = just collection of components (which are also presents data) + some primitive variables

    // Renderer = RenderTarget (texture or screen) + Camera
    // On scene there're many renderers

    class GameObject {
        private:
        std::unordered_map<std::string, ObjectComponent*> components_;
    };
}
