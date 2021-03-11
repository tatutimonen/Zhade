#version 330 core

out vec4 color;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};
uniform Material material;

void main()
{
    vec3 x = material.ambient * 1;
    //vec3 y = material.specular * 2;
    float z = material.shininess * 3;
    color = vec4(material.diffuse, 1.0f);
}
