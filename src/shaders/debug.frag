#version 460

//------------------------------------------------------------------------
// Outputs.

layout (location = 0) out vec4 fragOut;

//------------------------------------------------------------------------
// Inputs from previous pipeline stages.

in VERT_OUT {
    vec2 tex;
    flat uint instanceID;
} FragIn;

//------------------------------------------------------------------------
// Uniforms.

uniform sampler2DArray u_diffuseTexture;

//------------------------------------------------------------------------

void main()
{
    fragOut = texture(u_diffuseTexture, vec3(FragIn.tex, FragIn.instanceID));
}

//------------------------------------------------------------------------
