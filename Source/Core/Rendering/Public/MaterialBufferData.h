#pragma once
#include <types.h>
struct UMaterialBufferData final
{
	u32 albedoIndex;
	u32 normalIndex;
	u32 ormIndex;
	u32 emissiveIndex;
	u32 materialType;
	u32 samplerIndex;
};