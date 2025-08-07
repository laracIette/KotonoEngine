#pragma once
#include <vulkan/vulkan_core.h>
#include "glm_includes.h"
class KtViewport final
{
public:
	const glm::uvec2& GetExtent() const;
	const glm::ivec2& GetOffset() const;

	bool GetIsKeepAspectRatio() const;
	float GetAspectRatio() const;

	void SetExtent(const glm::uvec2& extent);
	void SetOffset(const glm::ivec2& offset);

	void SetIsKeepAspectRatio(const bool isKeepAspectRatio);
	void SetAspectRatio(const float aspectRatio);

	void CmdUse(VkCommandBuffer commandBuffer) const;

private:
	glm::uvec2 extent_;
	glm::ivec2 offset_;

	bool isKeepAspectRatio_;
	float aspectRatio_;
};

extern KtViewport WindowViewport;