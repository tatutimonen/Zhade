#version 460 core
#extension GL_ARB_bindless_texture : require
#extension GL_ARB_gpu_shader_int64 : require
#extension GL_ARB_shading_language_include : require

#include "common_defs.h"

//------------------------------------------------------------------------
// Outputs.

layout (location = 0) out vec4 FragColor;

//------------------------------------------------------------------------
// Inputs from previous pipeline stages.

in VERT_OUT {
    vec2 uv;
    flat uint drawID;
} In;

//------------------------------------------------------------------------
// Uniforms etc.

layout (binding = DRAW_METADATA_BINDING, std430) restrict readonly buffer DrawMeta {
    DrawMetadata b_meta[];
};

layout (binding = DIRECTIONAL_LIGHT_PROPS_BINDING, std430) restrict readonly buffer SunLight {
    DirectionalLightProperties b_sunLight;
};

layout (binding = DIRECTIONAL_LIGHT_DEPTH_TEXTURE_BINDING, std140) uniform SunLightDepthTexture {
    sampler2D u_sunDepthTexture;
};

//------------------------------------------------------------------------

void main()
{
    FragColor = texture(sampler2D(b_meta[In.drawID].textures.diffuse), In.uv);
}

//------------------------------------------------------------------------
