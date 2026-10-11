#pragma once
#include "Vertex.h"
#include <vector>

struct UMeshData final
{
	std::vector<UVertex> vertices;
	std::vector<u32> indices;
	f32 boundingSphereRadius;
};
