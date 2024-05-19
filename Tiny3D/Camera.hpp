// by andrew.la
#pragma once

#include "Core/Utils.hpp"
#include "Core/Math.hpp"
#include "GameObject.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace tiny3d {
    class Camera3D {
        public:
        virtual glm::mat4 GetProjectionMatrix(float apect_ratio) const = 0;
        virtual glm::mat4 GetViewMatrix() const = 0;

        protected:
        glm::vec4 position_{0.0f, 0.0f, 0.0f, 1.0f};
        glm::vec4 rotation_{0.0f, 0.0f, 0.0f, 1.0f};
        real32 zoom_{1.0f};
    };

    class PerspectiveCamera3D : public Camera3D {
        public:
        PerspectiveCamera3D() = default;
        PerspectiveCamera3D(const PerspectiveCamera3D& camera) = default;
        PerspectiveCamera3D(PerspectiveCamera3D&& camera) = default;

        PerspectiveCamera3D& operator=(const PerspectiveCamera3D& camera) = default;
        PerspectiveCamera3D& operator=(PerspectiveCamera3D&& camera) = default;

        glm::mat4 GetProjectionMatrix(float apect_ratio) const;
        glm::mat4 GetViewMatrix() const;

        inline glm::vec4& Position() {
            return position_;
        }

        inline const glm::vec4& Position() const {
            return position_;
        }

        inline glm::vec4& Rotation() {
            return rotation_;
        }

        inline const glm::vec4& Rotation() const {
            return rotation_;
        };

        private:        
        real32 fov_{80.0f};
        real32 z_near_{0.1f};
        real32 z_far_{100.0f};
    };

    class OrthoCamera3D : public Camera3D {};
}
