#version 410 core

struct PointLight {    
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  
#define NR_POINT_LIGHTS 1

in vec3 fPosition;
in vec3 fNormal;
in vec2 fTexCoords;

out vec4 fColor;

//matrices
uniform mat4 model;
uniform mat4 view;
uniform mat3 normalMatrix;

//lighting
uniform PointLight pls[NR_POINT_LIGHTS];
uniform vec3 lightDir;
uniform vec3 lightColor;

// textures
uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;

vec3 test() {
    return vec3(0.f, 0.f, 0.f);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 ld = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, ld), 0.0);
    
    // specular shading
    vec3 reflectDir = reflect(-ld, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.f);

    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
  			     light.quadratic * (distance * distance));    
    
    // combine results
    vec3 ambient  = vec3(0.f, 0.f, 0.f);
    vec3 diffuse  = light.diffuse  * diff * vec3(1.0f, 0.f, 0.f);
    vec3 specular = light.specular * spec * vec3(1.0f, 0.f, 0.f);
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
} 

vec3 computeDirLight()
{

    //components
    vec3 ambient;
    float ambientStrength = 0.2f;
    vec3 diffuse;
    vec3 specular;
    float specularStrength = 0.5f;

    //compute eye space coordinates
    vec4 fPosEye = view * model * vec4(fPosition, 1.0f);
    vec3 normalEye = normalize(normalMatrix * fNormal);

    //normalize light direction
    vec3 lightDirN = vec3(normalize(view * vec4(lightDir, 0.0f)));

    //compute view direction (in eye coordinates, the viewer is situated at the origin
    vec3 viewDir = normalize(- fPosEye.xyz);

    //compute ambient light
    ambient = ambientStrength * lightColor;

    //compute diffuse light
    diffuse = max(dot(normalEye, lightDirN), 0.0f) * lightColor;

    //compute specular light
    vec3 reflectDir = reflect(-lightDirN, normalEye);
    float specCoeff = pow(max(dot(viewDir, reflectDir), 0.0f), 32);
    specular = specularStrength * specCoeff * lightColor;

    return min((ambient + diffuse) * texture(diffuseTexture, fTexCoords).rgb + specular * texture(specularTexture, fTexCoords).rgb, 1.0f);
}

void main() 
{
    vec3 cl_final = computeDirLight();
    vec4 fPosEye = view * model * vec4(fPosition, 1.0f);
    vec3 viewDir = normalize(- fPosEye.xyz);

    for (int i = 0; i < NR_POINT_LIGHTS; ++i) {
       cl_final += CalcPointLight(pls[i], fNormal, fPosition, viewDir);
    }

    fColor = vec4(cl_final, 1.0f);
}
