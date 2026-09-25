#pragma once
#include <kotono_common/types.h>
#include <limits>

enum class ESceneVisibility : u32
{
	Mesh = 0x00000001,
	Bounds = 0x00000002,
	Collider = 0x00000004,
	Wireframe = 0x00000008,
	DirectionalLight = 0x00000010,
	PointLight = 0x00000020,
	All = std::numeric_limits<u32>::max(),
};