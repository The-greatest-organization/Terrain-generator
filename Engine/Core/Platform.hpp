#pragma once

#include "Utils.hpp"

namespace tiny3d {
    struct Platform {
        
        Platform();
        Platform(const Platform& platform) = delete;

        Platform& GetInstance();

        private:
        static Platform* platform_;

        // Allocators
        // Proc addresses of OpenGL functions
        // OpenGL constants
    };
}
