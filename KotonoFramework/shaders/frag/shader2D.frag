#version 460

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

layout(set = 0, binding = 1) uniform sampler2D texSampler;

void main() 
{
    const vec4 fragTexColor = texture(texSampler, fragTexCoord);
    outColor = vec4(fragColor * fragTexColor.rgb, fragTexColor.a);
    //outColor.rgb = pow(outColor.rgb, vec3(1.0 / 2.2)); // Gamma correction if using sRGB
}