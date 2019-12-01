//Toon Shading Fragment Shader
#version 330
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

out vec4 FragColor;
 
uniform vec3 lightPos;
uniform vec3 viewPos;
 
in vec3 world_pos;
in vec3 world_normal;
 
const vec3 ambinetColor = vec3(0.0, 0.40, 0.20);
 
//number of levels
//for diffuse color
int levels = 5;
float scaleFactor = 1.0 / levels;
vec3 diffuseColor = vec3(0.30,0.80,0.10);
vec3 color = vec3(0.90,0.0,0.20);
//vec3 specular = vec3(0.30,0.80,0.10);

float material_shininess = 100;
float material_kd = 0.5;
float material_ks = 0.3;

uniform Material material;
 
void main()
{ 
    vec3 L = normalize(lightPos - world_pos);
    vec3 V = normalize(viewPos - world_pos);
    
    float diffuse = max(0, dot(L, world_normal));
    diffuseColor = diffuseColor * material_kd * floor(diffuse * levels) * scaleFactor;
    
    vec3 H = normalize(L + V);
    
    float specular = 0.0;
    
    if (dot(L, world_normal) > 0.0)
    {
        specular = material_ks * pow(max(0, dot(H, world_normal)), material_shininess);
    }
    
    //limit specular
    float specMask = (pow(dot(H, world_normal), material_shininess) > 0.4) ? 1 : 0;
    
    float edgeDetection = (dot(V, world_normal) > 0.2) ? 1 : 0;
    
    color = edgeDetection * (color + diffuseColor + specular * specMask);
    
    FragColor = vec4(color, 1);
}