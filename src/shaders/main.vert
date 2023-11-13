#version 460 core
#extension GL_ARB_shading_language_include : require
precision highp float;
precision highp int;

#include "common_defs.h"

//------------------------------------------------------------------------
// Vertex attributes.

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_nrm;
layout (location = 2) in vec2 a_uv;

//------------------------------------------------------------------------
// Outputs.

out gl_PerVertex {
    vec4 gl_Position;
};

out VERT_OUT {
    vec2 uv;
    flat uint drawID;
} Out;

//------------------------------------------------------------------------
// Uniforms etc.

layout (binding = CAMERA_BINDING, std140) uniform Camera {
    mat3x4 VT;
    mat4 P;
} u_camera;

layout (binding = MODEL_BINDING, std140) readonly buffer Model {
    mat3x4 MT[];
} b_model;

layout (binding = DRAW_ID_2_MODEL_IDX_BINDING, std140) readonly buffer DrawID2ModelIdx {
    uint b_drawID2ModelIdx[];
};

//------------------------------------------------------------------------

void main()
{
    Out.uv = a_uv;
    Out.drawID = gl_DrawID;
    uint modelIdx = b_drawID2ModelIdx[gl_DrawID];
    vec3 modelPos = vec4(a_pos, 1.0) * b_model.MT[modelIdx];
    vec3 viewModel = vec4(modelPos, 1.0) * u_camera.VT;
    gl_Position = u_camera.P * vec4(viewModel, 1.0);
}

//------------------------------------------------------------------------
