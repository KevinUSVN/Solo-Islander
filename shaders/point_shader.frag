#version 330 core
out vec4 FragColor;

uniform vec3 color;
in vec3 fragmentColor;

void main()
{
    FragColor = vec4(fragmentColor, 1.0f);
} 