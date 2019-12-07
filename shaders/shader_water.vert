#version 330 core

//The vertex shader gets called once per vertex.

//Define the position
layout (location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;

//Define uniform MVP passed from the object.
uniform mat4 model, view, projection;

out vec3 pos;

out vec3 world_pos;
out vec3 world_normal;
out vec2 tex_coords;

// Outputs of the vertex shader are the inputs of the same name of the fragment shader.
// The default output, gl_Position, should be assigned something. You can define as many
// extra outputs as you need.

void main()
{
    pos = position;
    tex_coords = texCoords;

    world_pos = mat3(model) * position;//careful here
    world_normal = normalize(mat3(model) * normal);
    gl_Position = projection * view * model * vec4(position, 1.0);
}  