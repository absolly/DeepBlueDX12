#version 330 core

layout (location = 0) in vec3 position;
layout (location = 3) in mat4 instanceMatrix;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    gl_Position = projection * view * instanceMatrix * vec4(position, 1.0f); 
}