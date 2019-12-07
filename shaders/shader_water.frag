#version 330 core

out vec4 FragColor;

uniform samplerCube cubeMap;
uniform sampler2D terrain_water;
uniform sampler2D terrain_sand;
uniform sampler2D terrain_grass;
uniform sampler2D terrain_snow;
 
uniform vec3 lightPos;
uniform vec3 viewPos;
 
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
    // // Set basic colors
    // if (0 <= world_pos.y && world_pos.y < 1) //Water
    // {
    //     color = vec4(0.0, 0.0, 1.0, 1);
    // }

    if (10 == world_pos.y) //Water
    {
        color = vec4(0.0, 0.0, 1.0, 1);
    }

    FragColor = color;

    // To check normal colors
    // vec3 norm = (normalize(world_normal) + 1) / 2;
    // FragColor = vec4(norm, 1);
}