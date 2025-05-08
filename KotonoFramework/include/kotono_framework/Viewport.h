#pragma once
#include <vulkan/vulkan_core.h>
#include "glm_includes.h"
class KtViewport final
{
public:
	const glm::uvec2& GetExtent() const;
	const glm::ivec2& GetOffset() const;

	void SetExtent(const glm::uvec2& extent);
	void SetOffset(const glm::ivec2& offset);

	const float GetAspectRatio() const;

	void CmdUse(VkCommandBuffer commandBuffer) const;

private:
	// perchance change to glm::vec2
	glm::ivec2 _offset;
	glm::uvec2 _extent;
};

extern KtViewport WindowViewport;