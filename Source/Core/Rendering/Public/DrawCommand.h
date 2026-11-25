#pragma once

#include "Containers/Array.h"
#include "types.h"

#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float4.hpp>
#include <vulkan/vulkan_core.h>

struct UDrawCommand final
{
	u32 drawIndex;

	VkPipeline pipeline;

	VkDeviceAddress vertexBufferAddress;
	u32				indexCount;
	u32				firstIndex;
	VkRect2D		scissor;

	struct Material
	{
		u32 albedoIndex;
		u32 normalIndex;
		u32 ormIndex;
		u32 emissiveIndex;
		u32 materialType;
		u32 samplerIndex;
	} material;

	glm::mat4 modelMatrix;
	glm::mat4 normalMatrix;

	f32 sortKey;

	UArray<f32, 16>       scalars;
	UArray<glm::vec4, 16> vectors;
	UArray<u32, 16>       textures;
};
