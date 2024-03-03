#version 460 core
#extension GL_ARB_gpu_shader_int64 : require
#extension GL_ARB_shading_language_include : require

#include "common_defs.h"

//------------------------------------------------------------------------
// Vertex attributes.

layout (location = 0) in vec3 a_pos;

//------------------------------------------------------------------------
// Outputs.

out gl_PerVertex {
    vec4 gl_Position;
};

//------------------------------------------------------------------------
// Uniforms etc.

layout (binding = VIEW_PROJ_BINDING, std140) uniform ViewProjBlock {
    ViewProjMatrices u_viewProj;
};

layout (binding = DRAW_METADATA_BINDING, std430) restrict readonly buffer DrawMetadataBlock {
    DrawMetadata b_meta[];
};

//------------------------------------------------------------------------

void main()
{
    vec3 modelWorld = vec4(a_pos, 1.0) * b_meta[gl_DrawID].modelMatT;
    vec3 modelView = vec4(modelWorld, 1.0) * u_viewProj.viewMatT;
    gl_Position = u_viewProj.projMat * vec4(modelView, 1.0);
}

//------------------------------------------------------------------------
