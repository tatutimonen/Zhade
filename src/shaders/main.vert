#version 460 core
#extension GL_ARB_gpu_shader_int64 : require
#extension GL_ARB_shading_language_include : require

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
    vec4 shadowCoord;
    flat uint drawID;
} Out;

//------------------------------------------------------------------------
// Uniforms etc.

layout (binding = VIEW_PROJ_BINDING, std140) uniform ViewProj {
    ViewProjMatrices u_viewProj;
};

layout (binding = HALF_VECTOR_BINDING, std140) uniform HalfVector {
    vec3 u_halfVector;
};

layout (binding = DRAW_METADATA_BINDING, std430) restrict readonly buffer DrawMeta {
    DrawMetadata b_meta[];
};

layout (binding = DIRECTIONAL_LIGHT_SHADOW_MATRIX_BINDING, std140) uniform ShadowMatrix {
    mat4 u_shadowMat;
};

//------------------------------------------------------------------------

void main()
{
    Out.uv = a_uv;
    Out.drawID = gl_DrawID;
    vec3 modelPos = vec4(a_pos, 1.0) * b_meta[gl_DrawID].modelMatT;
    vec3 viewModel = vec4(modelPos, 1.0) * u_viewProj.viewMatT;
    Out.shadowCoord = u_shadowMat * vec4(modelPos, 1.0);
    gl_Position = u_viewProj.projMat * vec4(viewModel, 1.0);
}

//------------------------------------------------------------------------
