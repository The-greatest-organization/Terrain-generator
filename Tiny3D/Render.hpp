// by andrew.la
#pragma once

#include "Core/Utils.hpp"
#include "Core/Math.hpp"
#include "Core/Graphics.hpp"
#include "GameObject.hpp"
#include "Camera.hpp"
#include "Lights.hpp"

namespace tiny3d {
    struct MaterialBlinnPhong {
        glm::vec4 Diffuse;
        glm::vec4 Specular;
        real32 Shininess;
    };

    struct MaterialPBR {
        glm::vec4 Albedo;
        real32 Metallic;
        real32 Roughness;
        real32 AO;
    };

    interface Renderer {
        virtual ~Renderer() = default;
        virtual void RenderObject(GameObject* object) = 0;

        virtual void ApplyLight() = 0;

        virtual void ApplyCamera2D() = 0;
        virtual void ApplyCamera3D() = 0;
    };
};
