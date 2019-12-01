#version 330 core
layout (location = 0) in vec3 aPos; // the position variable has attribute position 0
layout(location = 1) in vec3 vertexColor;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

out vec3 fragmentColor;

void main()
{
    fragmentColor = vertexColor;
    gl_Position = projection * view  * vec4(aPos, 1.0);
} 