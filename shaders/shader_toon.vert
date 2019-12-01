//Phong Reflection Model Vertex Shader
#version 330
 
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
 
uniform mat4 model, view, projection;
 
//send them to fragment shader
out vec3 world_pos;
out vec3 world_normal;
 
void main()
{
    //convert in world coords
    world_pos = mat3(model) * position;//careful here
    world_normal = normalize(mat3(model) * normal);
    gl_Position = projection * view * model * vec4(position, 1.0);
}