// by andrew.la
#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Utils.hpp"
#include "Math.hpp"

#define BLACK glm::vec4{0.0, 0.0, 0.0, 0.0}
#define WHITE glm::vec4{1.0, 1.0, 1.0, 0.0}
#define RED glm::vec4{1.0, 0.0, 0.0, 0.0}
#define GREEN glm::vec4{0.0, 1.0, 0.0, 0.0}
#define BLUE glm::vec4{0.0, 0.0, 1.0, 0.0}
#define YELLOW glm::vec4{1.0, 1.0, 0.0, 0.0}
#define CYAN glm::vec4{0.0, 1.0, 1.0, 0.0}
#define MAGENTA glm::vec4{1.0, 0.0, 1.0, 0.0}
#define ORANGE glm::vec4{1.0, 0.5, 0.0, 0.0}
#define PURPLE glm::vec4{1.0, 0.0, 0.5, 0.0}
#define LIME glm::vec4{0.5, 1.0, 0.0, 0.0}
#define ACQUA glm::vec4{0.0, 1.0, 0.5, 0.0}
#define VIOLET glm::vec4{0.5, 0.0, 1.0, 0.0}
#define AZURE glm::vec4{0.0, 0.5, 1.0, 0.0}

/// @todo: write some exceptions here (opengl errors especially)
namespace tiny3d::errors {};

namespace tiny3d {
    enum class Topologies : GLenum {
        Points = GL_POINTS,
        Lines = GL_LINES,
        LineLoops = GL_LINE_LOOP,
        LineStrips = GL_LINE_STRIP,
        Triangles = GL_TRIANGLES,
        Quads = GL_QUADS,
    };

    /// @todo implement texture & sampler classes

    class Texture1D {};

    class Texture2D {};

    class Texture3D {};

    class Sampler {};

    /// @todo: create option to load raw sources or precompiled spirv
    
    struct ShaderSource {
        // Remove ctors and create static methods which creates object
        ShaderSource() = delete;

        ShaderSource(const ShaderSource& source);
        ShaderSource(ShaderSource&& source);

        ~ShaderSource();

        static ShaderSource LoadSource(const std::string& path);
        static ShaderSource LoadSource(const char* path);

        inline char** Source() noexcept {
            assert(source_ != nullptr);
            return &source_;
        }

        private:
        char* source_;
    };

    enum class ShaderTypes : GLenum {
        Vertex = GL_VERTEX_SHADER,
        Geometry = GL_GEOMETRY_SHADER,
        Fragment = GL_FRAGMENT_SHADER,
    };

    template <ShaderTypes Type>
    struct Shader {
        Shader() = delete;

        Shader(const Shader& shader) = default;
        Shader(Shader&& shader) = default;

        /// @todo: add ctos with const ref to ShaderSource class

        // Somethnig like this. And shader source can be created by different ways, like:
        // reading shader from file, from const char* variable, precompiled or raw file etc
        
        // Shader(const ShaderSource& source) {};

        Shader(char** code) {}

        Shader(const ShaderSource& source) {}
        Shader(ShaderSource&& source) {}

        ~Shader() {
            glDeleteShader(id_);
        }

        Shader& operator=(const Shader&) = default;
        Shader& operator=(Shader&&) = default;

        void Compile() {
            // if (id_ == 0)
            //     throw ... ;

            glCompileShader(id_);
        }

        private:
        GLuint id_;
    };

    using VertexShader = Shader<ShaderTypes::Vertex>;
    using GeometryShader = Shader<ShaderTypes::Geometry>;
    using FragmentShader = Shader<ShaderTypes::Fragment>;

    class ShaderProgram {
        public:
        ShaderProgram();

        ShaderProgram(const ShaderProgram& program);
        ShaderProgram(ShaderProgram&& program);

        ShaderProgram& operator=(const ShaderProgram& program);
        ShaderProgram& operator=(ShaderProgram&& program);

        template <ShaderTypes Type>
        void AttachShader(const Shader<Type>& shader);

        private:
        GLuint id_;

        bool vertex_{false};
        bool geometry_{false};
        bool fragment_{false};
    };

    template <typename T>
    concept VertexType = std::movable<T> && std::copyable<T> && std::destructible<T>;

    template <typename T>
    concept UniformType = std::movable<T> && std::copyable<T> && std::destructible<T> && std::is_class_v<T>;

    enum class BufferUsages : GLenum {
        StaticDraw = GL_STATIC_DRAW,
        DynamicDraw = GL_DYNAMIC_DRAW,
        StreamDraw = GL_STREAM_DRAW,
    };

    /// @todo somehow deal with code duplication here

    template <VertexType T>
    class VertexBuffer {
        public:
        VertexBuffer() = delete;

        VertexBuffer(const VertexBuffer<T>& buffer) = default;
        VertexBuffer(VertexBuffer<T>&& buffer) = default;

        VertexBuffer(const std::vector<T>& vertices, BufferUsages usage) : vertices_(vertices), usage_(usage) {
            AllocateVRAM();
        }

        VertexBuffer(std::vector<T>&& vertices, BufferUsages usage) : vertices_(vertices), usage_(usage) {
            AllocateVRAM();
        }

        ~VertexBuffer() {
            ReleaseVRAM();
        }

        VertexBuffer<T>& operator=(const VertexBuffer<T>& buffer) = default;
        VertexBuffer<T>& operator=(VertexBuffer<T>&& buffer) = default;

        void AllocateVRAM() {
            // if (id_ != 0)
            //     throw ...
            
            glGenBuffers(1, &id_);
            glBindBuffer(GL_ARRAY_BUFFER, id_);
            glBufferData(GL_ARRAY_BUFFER, sizeof(T), vertices_.data(), usage_);
        }

        void ReleaseVRAM() noexcept {
            glDeleteBuffers(1, &id_);
        }


        T* Get() noexcept {
            return vertices_.data();
        }

        const T* Get() const noexcept {
            return vertices_.data();
        }

        private:
        GLuint id_;
        std::vector<T> vertices_;
        BufferUsages usage_;
    };

    class IndexBuffer {
        public:
        IndexBuffer() = delete;

        IndexBuffer(const IndexBuffer& buffer) = delete;
        IndexBuffer(IndexBuffer&& buffer);

        IndexBuffer(const std::vector<uint32>& indices);
        IndexBuffer(std::vector<uint32>&& indices);
        
        ~IndexBuffer();

        IndexBuffer& operator=(const IndexBuffer& buffer);
        IndexBuffer& operator=(IndexBuffer&& buffer);

        void ReleaseRAM();
        void ReleaseVRAM();

        uint32* Get() noexcept;
        const uint32* Get() const noexcept;

        private:
        GLuint id_;
        std::vector<uint32> indices_;
    };

    template <UniformType T>
    class UniformBuffer {
        public:
        UniformBuffer() = delete;

        UniformBuffer(const UniformBuffer<T>& buffer);
        UniformBuffer(UniformBuffer<T>&& buffer);

        template <typename... Args>
        UniformBuffer(Args&&... args);

        ~UniformBuffer();

        UniformBuffer<T>& operator=(const UniformBuffer<T>& buffer);
        UniformBuffer<T>& operator=(UniformBuffer<T>&& buffer);

        T* Get() noexcept;
        const T* Get() const noexcept;

        void ReleaseRAM();
        void ReleaseVRAM();

        private:
        GLuint id_;
        T* object_;
    };
};
