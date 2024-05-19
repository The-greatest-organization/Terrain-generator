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
        ObjectComponentT(ObjectComponentT&& component) : ObjectComponent(std::move(component.Name())), data_(std::move(component.data_)) {}

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

    using Translation2D = ObjectComponentT<glm::vec2>;
    using Rotation2D = ObjectComponentT<glm::vec2>;
    using Scale2D = ObjectComponentT<glm::vec2>;

    using Translation3D = ObjectComponentT<glm::vec4>;
    using Rotation3D = ObjectComponentT<glm::vec4>;
    using Scale3D = ObjectComponentT<glm::vec4>;

    struct __transform2d {
        glm::vec2 translation;
        glm::vec2 rotation;
        glm::vec2 scale;
    };

    struct __transform3d {
        glm::vec4 translation;
        glm::vec4 rotation;
        glm::vec4 scale;
    };

    using Transform2D = ObjectComponentT<__transform2d>;
    using Transform3D = ObjectComponentT<__transform3d>;

    /// @todo implement graphics components

    // Graphics components will be looking someting like this
    // class RenderComponent : public ObjectComponent<RenderData> {};

    // Object = just collection of components (which are also presents data) + some primitive variables

    // Renderer = RenderTarget (texture or screen) + Camera
    // On scene there're many renderers

    /// @todo: implement functional components (on key pressed, on key release etc)

    class GameObject {
        public:
        enum class Types : uint32 {
            Object2D,
            Light2D,

            Object3D,
            Light3D,

            ObjectUI,
        };

        static std::string GetTypeString(Types type);

        GameObject() = delete;

        GameObject(const GameObject& object);
        GameObject(GameObject&& object) = default;

        GameObject(Types type, const std::string& name);
        GameObject(Types type, const std::string& name, std::initializer_list<ObjectComponent*> components);

        ~GameObject();

        GameObject& operator=(const GameObject& object);
        GameObject& operator=(GameObject&& object) = default;

        void AttachComponent(ObjectComponent* component);
        void AttachComponents(std::initializer_list<ObjectComponent*> components);

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

        inline Types Type() const noexcept {
            return type_;
        }

        protected:
        std::unordered_map<std::string, ObjectComponent*> components_{10};
        std::string name_;
        Types type_;
    };
}
