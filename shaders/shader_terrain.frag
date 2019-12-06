#version 330 core

out vec4 FragColor;

uniform samplerCube cubeMap;
uniform sampler2D terrain_texture;
 
uniform vec3 lightPos;
uniform vec3 viewPos;
 
in vec3 world_pos;
in vec3 world_normal;
 
//number of levels
//for diffuse color
int levels = 5;
float scaleFactor = 1.0 / levels;
vec3 diffuseColor = vec3(0.30, 0.80, 0.10);
vec3 color = vec3(0.90, 0.0, 0.20);
//vec3 specular = vec3(0.30,0.80,0.10);

float material_shininess = 100;
float material_kd = 0.5;
float material_ks = 0.3;

//out vec4 color;
in vec3 pos;
out vec2 tex;

void main()
{
    // Set basic colors
    if (0 <= world_pos.y && world_pos.y < 1) //Water
    {
        color = vec3(0.0, 0.5, 0.8);
        //texture(terrain_water, tex);
    }
    else if (1 <= world_pos.y && world_pos.y < 15) //Sand
    {
        color = vec3(0.76, 0.7, 0.5);
    }
    else if (15 <= world_pos.y && world_pos.y < 100) //Grass
    {
        color = vec3(0.4, 0.5, 0.2);
    }
    else //Snow
    {
	    color = vec3(1.0, 1.0, 1.0);
    }

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
    float specMask = ((pow(dot(H, world_normal), material_shininess) > 0.4) && (0 <= world_pos.y && world_pos.y < 1)) ? 1 : 0;
    
    //float edgeDetection = (dot(V, world_normal) > 0.2) ? 1 : 0;
    
    //color = edgeDetection * (color + diffuseColor + specular * specMask);
    
    color = (color + diffuseColor + specular * specMask);
    
    FragColor = vec4(color, 1);
}