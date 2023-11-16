#version 460 core
#extension GL_ARB_bindless_texture : require
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

layout (binding = TEXTURE_BINDING, std140) uniform TextureBuffer {
    MeshTextures b_tex[MAX_DRAWS];
};

//------------------------------------------------------------------------

void main()
{
    Color = texture(b_tex[In.drawID].diffuse, In.uv);
}

//------------------------------------------------------------------------
