// by andrew.la
#pragma once

#include "Core/Utils.hpp"
#include "Core/Math.hpp"
#include "Core/Graphics.hpp"
#include "GameObject.hpp"
#include "Camera.hpp"
#include "Lights.hpp"

namespace tiny3d {
    struct Material {
        real32 Shininess{0.0f};
        glm::vec4 Ambient{0.2f, 0.2f, 0.2f, 1.0f};
        glm::vec4 Diffuse{0.8f, 0.8f, 0.8f, 1.0f};
        glm::vec4 Specular{0.0f, 0.0f, 0.0f, 1.0f};
        glm::vec4 Emission{0.0f, 0.0f, 0.0f, 1.0f};
    };

    class Renderer {
        public:
        virtual ~Renderer() = default;

        // This virtual method is suppose to call base graphics methods
        virtual void RenderObject(GameObject* object, const std::string& type) = 0;

        protected:
        // Base graphics methods

        void ApplyModelMaterial(const Material& material);

        void ApplyModelTranslation(const glm::vec2& translation);
        void ApplyModelTranslation(const glm::vec4& translation);

        void ApplyModelRotation(const glm::vec2& rotation);
        void ApplyModelRotation(const glm::vec4& rotation);

        void ApplyModelScale(const glm::vec2& scale);
        void ApplyModelScale(const glm::vec4& scale);

        void ApplyProjection(const Camera2D& camera);
        void ApplyProjection(const PerspectiveCamera3D& camera);
        void ApplyProjection(const OrthoCamera3D& camera);

        void ApplyViewTransform(const Camera2D& camera);
        void ApplyViewTransform(const PerspectiveCamera3D& camera);
        void ApplyViewTransform(const OrthoCamera3D& camera);

        void Draw(Topologies topology);
        void DrawIndexed(Topologies topology, const std::vector<int32>& indices);
    };
};
