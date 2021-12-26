#version 460 core

//------------------------------------------------------------------------
// Inputs from previous pipeline stages.

in VERT_OUT {
    vec2 texCoords;
    vec3 lighting;
} FragIn;

//------------------------------------------------------------------------
// Uniforms.

uniform sampler2D u_colorTexture;

struct Material {
    vec3 emission;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};
uniform Material u_material;

//------------------------------------------------------------------------
// Computations.

void main()
{
    vec4 texTerm = texture(u_colorTexture, FragIn.texCoords);
    gl_FragColor = min(vec4(FragIn.lighting, 1.0f) * texTerm * vec4(u_material.diffuse, 1.0f), vec4(1.0f));
}

//------------------------------------------------------------------------
