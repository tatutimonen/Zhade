#version 460 core
#extension GL_ARB_bindless_texture : require
#extension GL_ARB_gpu_shader_int64 : require
#extension GL_ARB_shading_language_include : require
precision mediump float;
precision highp int;

#include "common_defs.h"

//------------------------------------------------------------------------
// Outputs.

layout (location = 0) out vec4 Color;

//------------------------------------------------------------------------
// Inputs from previous pipeline stages.

in VERT_OUT {
    vec2 uv;
    flat uint drawID;
} In;

//------------------------------------------------------------------------
// Uniforms etc.

layout (binding = DRAW_METADATA_BINDING, std430) restrict readonly buffer DrawMetadataBuffer {
    DrawMetadata b_meta[];
};

//------------------------------------------------------------------------

void main()
{
    Color = texture(sampler2D(b_meta[In.drawID].textures.diffuse), In.uv);
}

//------------------------------------------------------------------------
