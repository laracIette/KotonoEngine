#version 460

layout(location = 0) in vec4 color;

layout(location = 0) out vec4 outColor;

void main() 
{
    outColor = color;
    //outColor.rgb = pow(outColor.rgb, vec3(1.0 / 2.2)); // Gamma correction if using sRGB
}