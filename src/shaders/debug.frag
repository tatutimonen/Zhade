#version 460 core
#extension GL_ARB_bindless_texture : require
precision mediump float;
precision highp int;

//------------------------------------------------------------------------
// Outputs.

layout (location = 0) out vec4 Out;

//------------------------------------------------------------------------
// Inputs from previous pipeline stages.

in VERT_OUT {
    vec2 uv;
    flat uint drawID;
} In;

//------------------------------------------------------------------------
// Uniforms etc.

layout (binding = 3, std140) readonly buffer Texture {
    sampler2D diffuse[];
} b_tex;

//------------------------------------------------------------------------

void main()
{
    Out = texture(b_tex.diffuse[In.drawID], In.uv);
}

//------------------------------------------------------------------------
