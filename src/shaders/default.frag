#version 460 core

//------------------------------------------------------------------------

in VERT_OUT {
    vec2 texCoords;
} FragIn;

//------------------------------------------------------------------------

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

void main()
{
    vec4 texTerm = texture(u_colorTexture, FragIn.texCoords);
    gl_FragColor = texTerm * vec4(u_material.diffuse, 1.0f);
}

//------------------------------------------------------------------------
