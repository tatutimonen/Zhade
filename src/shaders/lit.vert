#version 460 core

//------------------------------------------------------------------------
// Vertex attributes.

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_texCoords;

//------------------------------------------------------------------------
// Outputs.

out VERT_OUT {
    vec2 texCoords;
    vec3 lighting;
} VertOut;

//------------------------------------------------------------------------
// Uniforms.

layout (binding = 0, std140) uniform AmbientLight {
    vec3 color;
    float intensity;
} u_ambientLight;

/*layout (binding = 1, std140) uniform PointLight {
    vec3 color;
    vec3 position;
    float shininess;
    float strength;
    float attenuationConstant;
    float attenuationLinear;
    float attenuationQuadratic;
};*/

uniform mat4 u_VP;
uniform mat4 u_M;

//------------------------------------------------------------------------
// Computations.

void main()
{
    VertOut.texCoords = a_texCoords;
    VertOut.lighting = u_ambientLight.color * u_ambientLight.intensity;
    gl_Position = u_VP * u_M * vec4(a_position, 1.0);
}

//------------------------------------------------------------------------
