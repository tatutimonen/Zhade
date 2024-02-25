#version 460 core
#extension GL_ARB_bindless_texture : require
#extension GL_ARB_gpu_shader_int64 : require
#extension GL_ARB_shading_language_include : require

#include "common_defs.h"

#define makeSampler2D(textureType) sampler2D(b_meta[In.drawID].textures.textureType)

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

layout (binding = DRAW_METADATA_BINDING, std430) restrict readonly buffer DrawMetadataBlock {
    DrawMetadata b_meta[];
};

layout (binding = DIRECTIONAL_LIGHT_PROPS_BINDING, std430) restrict readonly buffer SunLightBlock {
    DirectionalLightProperties b_sunLight;
};

layout (binding = DIRECTIONAL_LIGHT_DEPTH_TEXTURE_BINDING, std140) uniform SunLightDepthTextureBlock {
    sampler2DShadow u_sunLightDepthTexture;
};

//------------------------------------------------------------------------

void main()
{
    vec4 diffuse = texture(makeSampler2D(diffuse), In.uv);
    float shadowFactor = textureProj(u_sunLightDepthTexture, In.shadowCoord);
    FragColor = shadowFactor * 0.7 * diffuse + vec4(b_sunLight.ambient, 1.0) * diffuse;
}

//------------------------------------------------------------------------
