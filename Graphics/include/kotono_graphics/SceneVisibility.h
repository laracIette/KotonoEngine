#pragma once
#include <kotono_common/types.h>

enum class ESceneVisibility : u32
{
	None				= 0b0000'0000,
	Mesh				= 0b0000'0001,
	Bounds				= 0b0000'0010,
	Collider			= 0b0000'0100,
	Wireframe			= 0b0000'1000,
	DirectionalLight	= 0b0001'0000,
	PointLight			= 0b0010'0000,
	All					= 0b1111'1111,
};