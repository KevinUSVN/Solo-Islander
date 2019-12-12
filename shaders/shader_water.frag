#version 330 core

out vec4 FragColor;

uniform sampler2D terrain_water;
uniform samplerCube cubeMap;
uniform sampler2D dudvMap;
 
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform float moveFactor;

uniform bool water_on;
 
in vec3 world_pos;
in vec3 world_normal;
in vec2 tex_coords;
 
//number of levels
//for diffuse color
int levels = 5;
float scaleFactor = 1.0 / levels;
vec4 diffuseColor = vec4(0.30, 0.80, 0.10, 1);
vec4 color = vec4(0.0, 0.0, 1.0, 1);

float material_shininess = 100;
float material_kd = 0.5;
float material_ks = 0.3;

float waveStrength = 0.2;

in vec3 pos;

void main()
{
    // // Set basic colors
    // if (0 <= world_pos.y && world_pos.y < 1) //Water
    // {
    //     color = vec4(0.0, 0.0, 1.0, 1);
    // }

    vec2 distortion1 = (texture(dudvMap, vec2(tex_coords.x + moveFactor, tex_coords.y)).rg * 2.0 - 1.0) * waveStrength;
    vec2 distortion2 = (texture(dudvMap, vec2(-tex_coords.x + moveFactor, tex_coords.y + moveFactor)).rg * 2.0 - 1.0) * waveStrength;

    vec2 totalDistortion = distortion1 + distortion2;

    vec3 offset1 = vec3(totalDistortion, 1.0);

    vec3 I = normalize(world_pos - viewPos);
    vec3 R = reflect(I, normalize(world_normal));

    if (10 == world_pos.y) //Water
    {
        color = vec4(texture(cubeMap, R + offset1).rgb, 1.0);
    }

    if (water_on)
    {
        FragColor = color;
    }
    else
    {
        FragColor = vec4(0.0, 0.0, 1.0, 1.0);
    }
    

    // To check normal colors
    // vec3 norm = (normalize(world_normal) + 1) / 2;
    // FragColor = vec4(norm, 1);
}