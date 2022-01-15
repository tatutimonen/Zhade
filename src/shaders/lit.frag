#version 460 core

//------------------------------------------------------------------------
// Pre-compile definitions and constants.

#pragma optionNV(fastmath on)
#pragma optionNV(inline all)
#pragma optionNV(unroll all)

const int maxPointLights = 4;

//------------------------------------------------------------------------
// Outputs.

layout (location = 0) out vec4 fragOut;

//------------------------------------------------------------------------
// Inputs from previous pipeline stages.

in VERT_OUT {
    vec2 texCoords;
    vec3 normal;
    vec3 viewDir;
} FragIn;

//------------------------------------------------------------------------
// Uniforms.

uniform sampler2D u_colorTexture;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};
uniform Material u_material;

layout (binding = 2, std140) uniform AmbientLight {
    vec4 color;
} u_ambientLight;

layout (binding = 3, std140) uniform DirectionalLight {
    vec3 color;
    float strength;
    vec3 direction;
} u_directionalLight;

struct PointLight {
    vec3 color;
    vec3 position;
    float shininess;
    float strength;
    float attenuationConstant;
    float attenuationLinear;
    float attenuationQuadratic;
};
layout(binding = 4, std140) uniform PointLights {
    PointLight u_pointLights[maxPointLights];
};

//------------------------------------------------------------------------

void main()
{
    float gamma = 2.2;
    vec3 tex = pow(texture(u_colorTexture, FragIn.texCoords).rgb, vec3(gamma));

    float ambientStrength = u_ambientLight.color.a;
    vec3 ambient = tex * u_material.ambient * ambientStrength * u_ambientLight.color.rgb;

    float lambertian = max(dot(FragIn.normal, u_directionalLight.direction), 0.0);
    vec3 diffuse =  tex * u_material.diffuse * lambertian;
    
    vec3 specular = vec3(0.0);
    if (lambertian > 0.0)
    {
        vec3 halfway = normalize(FragIn.viewDir + u_directionalLight.direction);
        float specularFactor = pow(max(dot(FragIn.normal, halfway), 0.0), u_material.shininess);
        specular = tex * u_material.specular * specularFactor;
    }

    vec3 color = ambient + (diffuse + specular) * u_directionalLight.strength * u_directionalLight.color;
    fragOut = pow(vec4(color, 1.0), vec4(1.0 / gamma));
}

//------------------------------------------------------------------------
