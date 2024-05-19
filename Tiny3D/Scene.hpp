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
        SceneNode(const SceneNode& node) = delete;
        SceneNode(SceneNode&& node);

        SceneNode(GameObject* object);

        ~SceneNode();

        SceneNode& operator=(SceneNode&& node);

        inline bool& Visible();

        inline operator GameObject*() {
            return object_;
        }

        friend class Scene;

        private:
        void AddChild(SceneNode* node);

        SceneNode* RemoveChild(const std::string& name);

        SceneNode* GetChild(const std::string& name);

        std::unordered_map<std::string, SceneNode*> children_;

        GameObject* object_;

        bool visible_;
    };

    class Scene {
        Scene() = default;
        Scene(const Scene& scene);
        Scene(Scene&& scene);

        GameObject* FindObject(const std::string& name);
        GameObject* FindObjectRecursive(std::initializer_list<const string&> names);

        private:
        /// @todo create a different types of objects
        
        Renderer* render_3d;
        Renderer* render_2d;
        Renderer* render_ui;
    };
}
