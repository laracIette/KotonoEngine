#version 460
#include "../common_vert.glsl"

void main()
{
	UNPACK_PUSH_CONSTANTS

	gl_Position = directionalLight.lightViewProjs[gl_ViewIndex] * transform.modelMatrix * vec4(vertex.position, 1.0);
}