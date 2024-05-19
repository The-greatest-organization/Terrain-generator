#version 460 core

#define MAX_DIRECT_LIGHTS 10
#define MAX_POINT_LIGHTS 20

struct DirectLight {
    vec4 direction;

    vec4 diffuse;
    vec4 specular;
};

struct PointLight {
    vec4 position;

    float constant;
    float linear;
    float quadratic;

    vec4 diffuse;
    vec4 specular;
};

struct Material {
    uniform vec4 u_Color;
    uniform vec4 u_Roughness;
    uniform float u_Shininess;
};

in VS_OUT {
    vec4 FragPos;
    vec4 Normal;
    vec2 TexCoords;
} fs_in;

/// @todo: optimization with open_gl buffers

uniform Material u_Material;

uniform DirectLight u_DirectLights[MAX_DIRECT_LIGHTS];
uniform uint u_DirectLightsUsed;

uniform PointLight u_PointLights[MAX_POINT_LIGHTS];
uniform uint u_PointLightsUsed;

out vec4 gl_FragColor;

vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 pos, vec3 view) {
    vec3 light_dir = normalize(light.position - pos);
    vec3 normal_dir = normalize(normal);

    vec3 view_dir = normalize()
    vec3 halfway_dir = normalize(light_dir + view);

    float angle = max(dot(light_dir, normal_dir), 0.0);
    vec4 diffuse = u_Material.diffuse * angle;
}

vec3 CalculateDirectLight(DirectLight light, vec3 normal, vec3 view) {}

void main() {
    gl_FragColor = vec4(0.3882, 0.8314, 0.8392, 1.0);
}
