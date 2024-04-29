// by andrew.la
#pragma once

#include "Core/Utils.hpp"
#include "GameObject.hpp"
#include "Render.hpp"

#include <map>

namespace tiny3d::errors {
    struct NodeNotFound : std::exception {};
    struct NodeAlreadyExists : std::exception {};
}

namespace tiny3d {
    class SceneNode {
        public:
        SceneNode() = delete;
        SceneNode(const SceneNode& other) = delete;
        SceneNode(SceneNode&& other);

        SceneNode(GameObject* object);

        ~SceneNode();

        inline bool& Visible();

        inline operator GameObject*() {
            return object_;
        }

        friend class Scene;

        private:
        void AddChild(SceneNode* another);
        void AddChild(GameObject* object);

        SceneNode* RemoveChild(const std::string& name);

        SceneNode* GetChild(const std::string& name);

        std::unordered_map<std::string, SceneNode*> children_;
        
        GameObject* object_;

        bool visible_;

        std::string type_;
    };

    class Scene {
        Scene() = default;

        GameObject* FindObject(const std::string& name);
        GameObject* FindObjectRecursive(std::initializer_list<const string&> names);

        private:
        std::unordered_map<std::string, SceneNode*> objects_{10};

        Renderer* renderer_;
    };
}
