#version 460 core

//------------------------------------------------------------------------
// Pre-compile definitions.

#pragma optionNV(fastmath on)

//------------------------------------------------------------------------
// Vertex attributes.

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_texCoords;

//------------------------------------------------------------------------
// Outputs.

out VERT_OUT {
    vec2 texCoords;
} VertOut;

//------------------------------------------------------------------------
// Uniforms.

uniform mat4 u_M;
uniform mat4 u_VP;

//------------------------------------------------------------------------

void main()
{
    VertOut.texCoords = a_texCoords;
    gl_Position = u_VP * u_M * vec4(a_position, 1.0);
}

//------------------------------------------------------------------------
