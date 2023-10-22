#version 460 core
#extension GL_ARB_bindless_texture : require
precision mediump float;
precision highp int;

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

layout (binding = 2, std430) readonly buffer Texture {
    sampler2D diffuse[];
} b_tex;

//------------------------------------------------------------------------

void main()
{
    fragOut = texture(b_tex.diffuse[FragIn.instanceID], FragIn.uv);
}

//------------------------------------------------------------------------
