#version 460 core
#extension GL_ARB_shading_language_include : require
#extension GL_ARB_bindless_texture : require
precision mediump float;
precision highp int;

#include "bindings.h"

//------------------------------------------------------------------------
// Outputs.

layout (location = 0) out vec4 fragOut;

//------------------------------------------------------------------------
// Inputs from previous pipeline stages.

in VERT_OUT {
    vec2 uv;
    flat uint instanceID;
} FragIn;

//------------------------------------------------------------------------
// Uniforms etc.

layout (binding = TEXTURE_BINDING, std430) readonly buffer Texture {
    sampler2D diffuse[];
} b_tex;

//------------------------------------------------------------------------

void main()
{
    fragOut = texture(b_tex.diffuse[FragIn.instanceID], FragIn.uv);
}

//------------------------------------------------------------------------
