#version 330 core

out vec4 FragColor;

uniform samplerCube cubeMap;
uniform sampler2D terrain_water;
uniform sampler2D terrain_sand;
uniform sampler2D terrain_grass;
uniform sampler2D terrain_snow;
 
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform bool toon_on;
 
in vec3 world_pos;
in vec3 world_normal;
 
//number of levels
//for diffuse color
int levels = 5;
float scaleFactor = 1.0 / levels;
vec4 diffuseColor = vec4(0.30, 0.80, 0.10, 1);
vec4 color = vec4(0.90, 0.0, 0.20, 1);

float material_shininess = 100;
float material_kd = 0.5;
float material_ks = 0.3;

in vec3 pos;
in vec2 tex_coords;

void main()
{
    // Set basic colors
    if (0 <= world_pos.y && world_pos.y < 10) //Water
    {
        color = vec4(0.2, 0.2, 0.3, 1);
        //color = texture(terrain_water, tex_coords);
    }
    else if (10 <= world_pos.y && world_pos.y < 15) //Sand
    {
        color = vec4(0.76, 0.7, 0.5, 1);
        //color = texture(terrain_sand, tex_coords);
    }
    else if (15 <= world_pos.y && world_pos.y < 100) //Grass
    {
        color = vec4(0.4, 0.5, 0.2, 1);
        //color = texture(terrain_grass, tex_coords);
    }
    else //Snow
    {
	    color = vec4(1.0, 1.0, 1.0, 1);
        //color = texture(terrain_snow, tex_coords);
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

    // float edge = dot(normalize(V - world_pos), world_normal);
    // edge = max(0, edge);

    // if (edge < 0.001)
    // {
    //     color = 0.5 * color;
    // }
    
    if (toon_on)
    {
        color = (color + diffuseColor + specular * specMask);
    }
    

    FragColor = color;

    // To check normal colors
    // vec3 norm = (normalize(world_normal) + 1) / 2;
    // FragColor = vec4(norm, 1);
}