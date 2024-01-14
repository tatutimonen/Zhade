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
    vec4 shadowCoord;
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
    sampler2DShadow u_sunLightDepthTexture;
};

//------------------------------------------------------------------------

#define makeSampler2D(textureType) sampler2D(b_meta[In.drawID].textures.textureType)

void main()
{
    float shadowFactor = textureProj(u_sunLightDepthTexture, In.shadowCoord);
    FragColor = shadowFactor * texture(makeSampler2D(diffuse), In.uv);
}

//------------------------------------------------------------------------
