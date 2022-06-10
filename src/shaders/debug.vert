#version 460 core

//------------------------------------------------------------------------
// Vertex attributes.

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_nrm;
layout (location = 2) in vec2 a_tex;

//------------------------------------------------------------------------
// Outputs.

out VERT_OUT {
    vec2 tex;
} VertOut;

//------------------------------------------------------------------------
// Uniforms.

layout (binding = 0, std140) uniform Camera {
    mat4 V;
    mat4 P;
} u_camera;

layout (binding = 1, std140) uniform Model {
    mat4 M;
} u_model;

//------------------------------------------------------------------------

void main()
{
    VertOut.tex = a_tex;
    gl_Position = u_camera.P * u_camera.V * u_model.M * vec4(a_pos, 1.0);
}

//------------------------------------------------------------------------
