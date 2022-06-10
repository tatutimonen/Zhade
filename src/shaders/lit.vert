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
    vec3 normal;
    vec3 viewDir;
} VertOut;

//------------------------------------------------------------------------
// Uniforms.

layout (binding = 0, std140) uniform Camera {
    mat4 V;
    mat4 P;
} u_camera;

layout (binding = 1, std140) uniform Model {
    mat4 M;
    mat4 N;
} u_model;

//------------------------------------------------------------------------

void main()
{
    VertOut.texCoords = a_texCoords;
    VertOut.normal = normalize(mat3(u_model.N) * a_normal);
    mat4 MV = u_camera.V * u_model.M;
    VertOut.viewDir = -normalize(mat3(MV) * a_position);
    gl_Position = u_camera.P * MV * vec4(a_position, 1.0);
}

//------------------------------------------------------------------------
