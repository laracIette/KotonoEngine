#pragma once

#include "Containers/List.h"
#include "Vertex.h"

struct UMeshData final
{
	UList<UVertex> vertices;
	UList<u32> indices;
	f32 boundingSphereRadius;
};
