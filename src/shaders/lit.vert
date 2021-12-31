#version 460 core

//------------------------------------------------------------------------
// Pre-compile definitions.

#pragma optionNV(fastmath on)

//------------------------------------------------------------------------
// Vertex attributes.

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_texCoords;

//------------------------------------------------------------------------
// Outputs.

out VERT_OUT {
    vec2 texCoords;
    vec3 normal;
    vec3 cameraDirection;
} VertOut;

//------------------------------------------------------------------------
// Uniforms.

layout (binding = 0, std140) uniform Camera {
    vec3 center;
    mat4 V;
    mat4 P;
} u_camera;

uniform mat4 u_M;
uniform mat4 u_N;

//------------------------------------------------------------------------

void main()
{
    VertOut.texCoords = a_texCoords;
    VertOut.normal = normalize(mat3(u_N) * a_normal);
    VertOut.cameraDirection = normalize(u_camera.center - a_position);
    gl_Position = u_camera.P * u_camera.V * u_M * vec4(a_position, 1.0);
}

//------------------------------------------------------------------------
