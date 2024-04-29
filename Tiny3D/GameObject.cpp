#include "GameObject.hpp"

namespace tiny3d::errors {
    ComponentNotAttached::ComponentNotAttached(const std::string& name) : 
        log_("Component with this name is not attached: " + name) {}
    
    const char* ComponentNotAttached::what() const noexcept {
        return log_.c_str();
    }

    ComponentAlreadyAttached::ComponentAlreadyAttached(const std::string& name) : 
        log_("Component with this name are already attached" + name) {}
    
    const char* ComponentAlreadyAttached::what() const noexcept {
        return log_.c_str();
    }
}

namespace tiny3d {
    ObjectComponent::ObjectComponent(const std::string& name) : name_(name) {}

    GameObject::GameObject(const std::string& name, const std::string& type) : name_(name), type_(type) {}

    GameObject::GameObject(const std::string& name, const std::string& type, std::initializer_list<ObjectComponent*> components) {
        name_ = name;
        type_ = type;

        for (auto&& i : components) {
            assert(i != nullptr);
            components_.emplace(i->Name(), i->Copy());
        }
    }

    GameObject::GameObject(const GameObject& object) : name_(object.name_), type_(object.type_) {
        for (auto&& i : object.components_) {
            assert(i.second != nullptr);
            components_.emplace(i.first, i.second->Copy());
        }
    }

    GameObject::~GameObject() {
        ClearComponents();
    }

    GameObject& GameObject::operator=(const GameObject& object) {
        ClearComponents();

        for (auto&& i : object.components_) {
            assert(i.second != nullptr);
            components_.emplace(i.first, i.second->Copy());
        }

        return *this;
    }

    void GameObject::AttachComponent(ObjectComponent* component) {
        auto found = components_.find(component->Name());

        if (found != components_.end()) {
            assert(found->second != nullptr);
            throw errors::ComponentAlreadyAttached(component->Name());
        }

        components_.emplace(component->Name(), component);
    }

    void GameObject::AttachComponents(std::initializer_list<ObjectComponent*> components) {
        for (ObjectComponent* i : components)
            AttachComponent(i);
    }

    ObjectComponent* GameObject::DetachComponent(const std::string& name) {
        auto found = components_.find(name);

        if (found == components_.end())
            throw errors::ComponentNotAttached(name);

        assert(found->second != nullptr);
        
        ObjectComponent* ret = found->second;
        components_.erase(found);

        return ret;
    }

    void GameObject::ClearComponents() {
        while (!components_.empty()) {
            assert(components_.begin()->second != nullptr);

            delete components_.begin()->second;
            components_.erase(components_.begin());
        }
    }
}
