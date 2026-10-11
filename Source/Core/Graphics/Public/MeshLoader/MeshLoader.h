#pragma once
#include "MeshData.h"
#include <Containers/Map.h>
#include <Path/Path.h>

class SMeshLoader final
{
public:
	static auto Load(UPath const& path) -> UMeshData;
	
private:
	static UMap<UPath, UMeshData> meshDatas_;
};
