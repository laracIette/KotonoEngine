#version 460
#extension GL_EXT_nonuniform_qualifier : enable

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) flat in uint textureIndex;
layout(location = 3) in vec4 color;

layout(location = 0) out vec4 outColor;

layout(set = 1, binding = 0) uniform sampler2D textures[]; // bindless texture array

void main() 
{
    const vec4 fragTexColor = texture(textures[nonuniformEXT(textureIndex)], fragTexCoord);
    outColor = vec4(fragColor * fragTexColor.rgb, fragTexColor.a) * color;
    //outColor.rgb = pow(outColor.rgb, vec3(1.0 / 2.2)); // Gamma correction if using sRGB
}