#version 460 core

//------------------------------------------------------------------------
// Pre-compile definitions.

#pragma optionNV(fastmath on)
#pragma optionNV(inline all)
#pragma optionNV(unroll all)

#define MAX_POINT_LIGHTS 4

//------------------------------------------------------------------------
// Outputs.

layout (location = 0) out vec4 fragOut;

//------------------------------------------------------------------------
// Inputs from previous pipeline stages.

in VERT_OUT {
    vec2 texCoords;
    vec3 normal;
    vec3 cameraDirection;
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

layout (binding = 1, std140) uniform AmbientLight {
    vec4 color;
} u_ambientLight;

layout (binding = 2, std140) uniform DirectionalLight {
    vec3 color;
    float shininess;
    vec3 direction;
    float strength;
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
layout(binding = 3, std140) uniform PointLights {
    PointLight u_pointLights[MAX_POINT_LIGHTS];
};

//------------------------------------------------------------------------

void main()
{
    vec3 tex = texture(u_colorTexture, FragIn.texCoords).rgb;
    
    vec3 ambient = u_ambientLight.color.a * u_ambientLight.color.rgb * tex * u_material.ambient;

    float diffuseFactor = pow(max(0.0, dot(FragIn.normal, u_directionalLight.direction)), u_material.shininess);
    vec3 diffuse = u_directionalLight.color * diffuseFactor * tex * u_material.diffuse;
    
    vec3 halfway = normalize(FragIn.cameraDirection + u_directionalLight.direction);
    float specularFactor = u_directionalLight.strength \
        * pow(max(0.0, dot(FragIn.normal, halfway)), u_directionalLight.shininess);
    vec3 specular = u_directionalLight.color * specularFactor * tex * u_material.specular;

    vec3 lighting = ambient + diffuse + specular;
    float gamma = 2.4;
    fragOut = pow(vec4(lighting, 1.0), vec4(1.0 / gamma));
}

//------------------------------------------------------------------------
