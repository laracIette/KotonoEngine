#version 460
#include "../common_vert.glsl"

void main()
{
	UNPACK_PUSH_CONSTANTS

	gl_Position = directionalLight.lightViewProj * transform.modelMatrix * vec4(vertex.position, 1.0);
}