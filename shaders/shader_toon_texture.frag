
#version 330 core
// This is a sample fragment shader.

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
in float sampleExtraOutput;
in vec3 Normal;
in vec3 FragPos;
in vec3 LightPos;

uniform vec3 viewPos;
uniform vec3 color;
uniform vec3 lightColor;
uniform vec3 ambient;
uniform vec3 diffuse;
uniform vec3 specular;
uniform float shininess;
uniform float attenuation_strength;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 fragColor;

void main()
{
    // Use the color passed in. An alpha of 1.0f means it is not transparent.
    // ambient
    vec3 ambientStrength = normalize(ambient);
    vec3 ambients = ambientStrength * lightColor;

    // diffuse
    vec3 norm = (normalize(Normal)+1)/2 ;
    vec3 diffuseStrength = normalize(diffuse);
    vec3 proj_model = LightPos - FragPos;
    vec3 lightDir = normalize(proj_model);
    float diff = max(dot(Normal,lightDir), 0.0);
    vec3 diffuses = lightColor * (diff * diffuseStrength);

    // specular
    vec3 specularStrength = normalize(specular);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir),0.0), shininess);
    vec3 speculars = lightColor * (spec * specularStrength);

    // Attenuation
    float distance = length(LightPos - FragPos);
    float Attenuation = 1.0f / (attenuation_strength * distance);

    vec3 result = (norm);
    fragColor = vec4(result, sampleExtraOutput);
}