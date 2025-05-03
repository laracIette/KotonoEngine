#version 460

layout(location = 0) out vec4 outColor;

void main() 
{
    outColor = vec4(vec3(1.0), 0.1);
    //outColor.rgb = pow(outColor.rgb, vec3(1.0 / 2.2)); // Gamma correction if using sRGB
}