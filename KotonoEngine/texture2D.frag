#version 430 core

in vec2 TexCoords;

out vec4 FragColor;

uniform vec4 color;
uniform sampler2D texSampler;

void main()
{
    FragColor = texture(texSampler, TexCoords) * color;
}