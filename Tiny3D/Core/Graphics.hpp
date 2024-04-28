// by andrew.la
#pragma once

#include "Utils.hpp"
#include "Math.hpp"

/// @todo: write some exceptions here

namespace tiny3d::errors {};

namespace tiny3d {
    class Shader {
        // TODO: shader source here, virtual method 'compile'
    };

    class VertexShader : public Shader {
        // TODO: implementing virtual methods compile
    };

    class GeometryShader : public Shader {
        // TODO: implementing virtual methods compile
    };

    class FragmentShader : public Shader {
        // TODO: implementing virtual methods compile
    };

    class ShaderProgram {
        // TODO: method 'link & validate'
    };

    enum class TextureFilter : usize {
        Linear,
        Nearest,
    };

    enum class TextureAddressMode : usize {
        Repeat,
        MirrorRepeat,
        Clamp,
        Border,
    };

    struct TextureParams {
        TextureAddressMode addressMode = TextureAddressMode::Border;
        TextureFilter minFilter = TextureFilter::Nearest;
        TextureFilter magFilter = TextureFilter::Nearest;

        real32 maxAnitrosopy = 0.0f;

        real32 minLOD = -1000;
        real32 maxLOD = 1000;
        real32 lodBias = 0;
    };

    class Texture1D {};
    class Texture2D {};
    class Texture3D {};
    class TextureCubemap {};

    enum class BufferUsage : usize {
        StaticUsage,
        StreamUsage,
        DynamicUsage,
    };

    class VertexBuffer {};
    class IndexBuffer {};
    class UniformBuffer {};

    class RenderTarget {};
};
