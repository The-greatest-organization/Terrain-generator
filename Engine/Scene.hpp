#pragma once

#include "Utils.hpp"

namespace tiny3d {
    // Scene = a linked list

    // first_____node
    //    /        |
    // second <- child
    //    /|
    //  nullptr

    class SceneNode {
        public:
        friend class Scene;

        SceneNode() = delete;
        SceneNode(const SceneNode& other) = delete;
        SceneNode(SceneNode&& other) = delete;

        void AddChild(SceneNode* another);

        void MakeInvisible() noexcept;
        void MakeVisible() noexcept;

        private:
        SceneNode* next_ {nullptr};
        SceneNode* child_ {nullptr};

        bool is_visible_ {false};
    };

    class Scene {
        private:
        SceneNode* head_;
    };
}
