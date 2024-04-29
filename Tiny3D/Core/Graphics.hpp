// by andrew.la
#pragma once

#include "Utils.hpp"
#include "Math.hpp"
#include <GLFW/glfw3.h>

/// @todo: write some exceptions here (opengl errors especially)

namespace tiny3d::errors {};

namespace tiny3d {
    enum class Topologies : GLenum {
        Points = GL_POINTS,
        Lines = GL_LINES,
        LineLoops = GL_LINE_LOOP,
        LineStrips = GL_LINE_STRIP,
        Triangles = GL_TRIANGLES,
    };

    class Texture1D {};

    class Texture2D {};

    class Texture3D {};

    template <std::destructible T>
    class VertexBuffer {
        VertexBuffer() = delete;
        
        VertexBuffer(const VertexBuffer<T>& buffer);
        VertexBuffer(VertexBuffer<T>&& buffer);
        
        VertexBuffer(std::initializer_list<T> vertices);
        VertexBuffer(const std::vector<T>& vertices);

        VertexBuffer<T>& operator=(const VertexBuffer<T>& buffer);
        VertexBuffer<T>& operator=(VertexBuffer<T>&& buffer);

        inline T* Data() const noexcept {
            return vertices_;
        }

        private:
        T* vertices_;
    };
};
