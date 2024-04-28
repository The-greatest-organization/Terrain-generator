// by andrew.la
#pragma once

#include "Core/Math.hpp"
#include "GameObject.hpp"

namespace tiny3d {

    /// @todo add 'calculate_projection_matrix' method

    class Camera2D : public GameObject {};
    
    class PerspectiveCamera3D : public GameObject {};
    
    class OrthoCamera3D : public GameObject {};
}
