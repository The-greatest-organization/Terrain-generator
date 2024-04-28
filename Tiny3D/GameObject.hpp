// by andrew.la
#pragma once

#include "Core/Utils.hpp"
#include "Core/Math.hpp"

#include <unordered_map>

namespace tiny3d::errors {
    struct ComponentNotAttached : std::exception {
        ComponentNotAttached() = delete;
        ComponentNotAttached(const std::string& name);

        const char* what() const noexcept override;

        private:
        std::string log_;
    };

    struct ComponentAlreadyAttached : std::exception {
        ComponentAlreadyAttached() = delete;
        ComponentAlreadyAttached(const std::string& name);

        const char* what() const noexcept override;

        private:
        std::string log_;
    };
};

namespace tiny3d {
    template <typename T>
    concept ComponentType = std::movable<T> && std::copyable<T> && std::destructible<T>;

    struct ObjectComponent {
        ObjectComponent() = delete;
        ObjectComponent(const ObjectComponent&) = default;
        ObjectComponent(ObjectComponent&&) = default;

        ObjectComponent(const std::string& name);

        virtual ~ObjectComponent() = default;

        inline const std::string& Name() const noexcept {
            return name_;
        }

        virtual ObjectComponent* Copy() = 0;

        private:
        std::string name_;
    };

    template <ComponentType T>
    class ObjectComponentT : public ObjectComponent {
        public:
        using Type = T;

        ObjectComponentT() = delete;
        ObjectComponentT(const ObjectComponentT& component) : ObjectComponent(component.Name()), data_(component.data_) {}
        ObjectComponentT(ObjectComponentT&& component) : data_(std::move(component.data_)) {}

        template <typename... Args>
        ObjectComponentT(const std::string& name, Args&&... args) : ObjectComponent(name), data_(std::forward<Args>(args)...) {}

        template <typename U>
        ObjectComponentT(const std::string& name, U&& ref) : ObjectComponent(name) {
            static_assert(std::is_same_v<typename std::remove_reference_t<U>, T>);
            data_ = T{std::forward<U>(ref)};
        }

        virtual ~ObjectComponentT() = default;

        inline T& Get() noexcept {
            return data_;
        }

        ObjectComponent* Copy() {
            return new ObjectComponentT<T>(Name(), data_);
        }

        protected:
        T data_;
    };

    class Translation2D : public ObjectComponentT<glm::vec2> {
        public:
        using ObjectComponentT<glm::vec2>::ObjectComponentT;
        glm::mat3 CalculateMatrix() const noexcept;
    };

    class Rotation2D : public ObjectComponentT<glm::vec2> {
        public:
        using ObjectComponentT<glm::vec2>::ObjectComponentT;
        glm::mat3 CalculateMatrix() const noexcept;
    };

    class Scale2D : public ObjectComponentT<glm::vec2> {
        public:
        using ObjectComponentT<glm::vec2>::ObjectComponentT;
        glm::mat3 CalculateMatrix() const noexcept;
    };

    class Translation3D : public ObjectComponentT<glm::vec4> {
        public:
        using ObjectComponentT<glm::vec4>::ObjectComponentT;
        glm::mat3 CalculateMatrix() const noexcept;
    };

    class Rotation3D : public ObjectComponentT<glm::vec4> {
        public:
        using ObjectComponentT<glm::vec4>::ObjectComponentT;
        glm::mat3 CalculateMatrix() const noexcept;
    };

    class Scale3D : public ObjectComponentT<glm::vec4> {
        public:
        using ObjectComponentT<glm::vec4>::ObjectComponentT;
        glm::mat3 CalculateMatrix() const noexcept;
    };

    /// @todo implement graphics components

    // Graphics components will be looking someting like this
    // class RenderComponent : public ObjectComponent<RenderData> {};

    // Object = just collection of components (which are also presents data) + some primitive variables

    // Renderer = RenderTarget (texture or screen) + Camera
    // On scene there're many renderers

    /// @todo: implement functional components (on key pressed, on key release etc)

    class GameObject {
        public:
        /// @todo Rule of 5
        GameObject() = delete;

        GameObject(const GameObject& object);
        GameObject(GameObject&& object) = default;

        GameObject(const std::string& name);
        GameObject(const std::string& name, std::initializer_list<ObjectComponent*> components);

        ~GameObject();

        GameObject& operator=(const GameObject& object);
        GameObject& operator=(GameObject&& object) = default;

        void AttachComponent(ObjectComponent* component);
        ObjectComponent* DetachComponent(const std::string& name);

        template <typename T>
        T* GetComponent(const std::string& name) const {
            static_assert(std::is_base_of_v<ObjectComponent, T>);

            auto found = components_.find(name);

            if (found == components_.end())
                throw errors::ComponentNotAttached(name);

            assert(found->second != nullptr);

            return reinterpret_cast<T*>(found->second);
        }

        void ClearComponents();

        inline const std::string& Name() const noexcept {
            return name_;   
        }

        protected:
        std::unordered_map<std::string, ObjectComponent*> components_{10};
        std::string name_;
    };
}
