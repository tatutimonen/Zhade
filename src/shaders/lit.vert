#version 460 core

//------------------------------------------------------------------------

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_texCoords;

//------------------------------------------------------------------------

out VERT_OUT {
    vec2 texCoords;
} VertOut;

//------------------------------------------------------------------------

layout (binding = 0, std140) uniform PointLightSettings {
    vec3 color;
    vec3 position;
    float shininess;
    float strength;
    float attenuationConstant;
    float attenuationLinear;
    float attenuationQuadratic;
};

uniform mat4 u_VP;
uniform mat4 u_M;

//------------------------------------------------------------------------

void main()
{
    gl_Position = u_VP * u_M * vec4(a_position, 1.0);
    VertOut.texCoords = a_texCoords;
}

//------------------------------------------------------------------------
