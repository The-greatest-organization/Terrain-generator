#include "Camera.hpp"

namespace tiny3d {
    glm::mat4 PerspectiveCamera3D::GetProjectionMatrix(float aspect_ratio) const {
        return glm::perspective(fov_, aspect_ratio, z_near_, z_far_);
    }

    glm::mat4 PerspectiveCamera3D::GetViewMatrix() const {
        glm::mat4 mat;
        mat[3] = -position_;
        mat[3][3] = 1;

        mat = glm::rotate(mat, rotation_[0], glm::vec3{1, 0, 0});
        mat = glm::rotate(mat, rotation_[1], glm::vec3{0, 1, 0});
        mat = glm::rotate(mat, rotation_[2], glm::vec3{0, 0, 1});

        return mat;
    }
}
