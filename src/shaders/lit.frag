#version 460 core

//------------------------------------------------------------------------
// Pre-compile definitions.

#pragma optionNV(fastmath on)
#pragma optionNV(inline all)
#pragma optionNV(unroll all)

#define MAX_POINT_LIGHTS 4

//------------------------------------------------------------------------
// Outputs.

layout (location = 0) out vec4 fragOut;

//------------------------------------------------------------------------
// Inputs from previous pipeline stages.

in VERT_OUT {
    vec2 texCoords;
} FragIn;

//------------------------------------------------------------------------
// Uniforms.

uniform mat4 u_P;

uniform sampler2D u_colorTexture;

struct Material {
    vec3 emission;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};
uniform Material u_material;

layout (binding = 0, std140) uniform AmbientLight {
    vec3 color;
    float intensity;
} u_ambientLight;

layout (binding = 1, std140) uniform DirectionalLight {
    vec3 color;
    float shininess;
    vec3 direction;
    float strength;
} u_directionalLight;

struct PointLight {
    vec3 color;
    vec3 position;
    float shininess;
    float strength;
    float attenuationConstant;
    float attenuationLinear;
    float attenuationQuadratic;
};
layout(binding = 2, std140) uniform PointLights {
    PointLight u_pointLights[MAX_POINT_LIGHTS];
};

//------------------------------------------------------------------------

void main()
{
    // For a pinhole camera, it holds P = [K | -t] => [K | -t] * vec4(c, 1.0) = 0 <=> c = t
    // vec4 cameraCenter = -u_P[3];
    // vec4 halfway = normalize(u_directionalLight.direction + cameraCenter);
    vec4 texTerm = texture(u_colorTexture, FragIn.texCoords);
    fragOut = min(vec4(u_ambientLight.color, 1.0) * texTerm * vec4(u_material.diffuse, 1.0f), vec4(1.0f));
}

//------------------------------------------------------------------------
