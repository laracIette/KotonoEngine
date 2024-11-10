#version 430 core

out vec2 TexCoords;

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

uniform mat4 model;

void main()
{
    gl_Position = vec4(aPos, 1.0) * model;
    TexCoords = aTexCoords;
}