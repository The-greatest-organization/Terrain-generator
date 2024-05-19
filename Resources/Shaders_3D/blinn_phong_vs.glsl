#version 460 core

layout (location = 0) in vec4 in_Position;
layout (location = 1) in vec4 in_Normal;
layout (location = 2) in vec2 in_TexCoords;

in VS_OUT {
    vec4 FragPos;
    vec4 Normal;
    vec2 TexCoords;
} vs_out;

/// @todo: optimizations

uniform mat4 u_ModelMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjectionMatrix;

void main() {
    vec4 calculated = u_ProjectionMatrix * u_ViewMatrix * u_ModelMatrix * in_Position;

    vs_out.FragPos = calculated;
    vs_out.Normal = in_Normal;
    vs_out.TexCoords = in_TexCoords;

    gl_Position = vec4(calculated.xyz, 1.0);
}
