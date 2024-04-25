// by andrew.la
#pragma once

#include "Core/Math.hpp"
#include "Component.hpp"

namespace tiny3d {

    /// @todo add 'calculate_projecti_matrix' method

    class PerspectiveCamera3D {
        private:
        Translation3D translation_;
        Rotation3D rotation_; // {pitch, yaw, roll}
        
        real32 fov_ {PI32 / 2.0f}; // field of view in radian
        real32 zoom_ {1.0f};
    };

    class OrthoCamera3D {
        private:
        Translation3D translation_;
        Rotation3D rotation_;
        real32 zoom_ {1.0f};
    };

    class Camera2D {
        private:
        Translation2D translation_;
        Rotation2D rotation_;
        real32 zoom_ {1.0f};
    };
}
