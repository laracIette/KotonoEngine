#pragma once
#include <vulkan/vulkan_core.h>
class KtViewport final
{
public:
	const VkOffset2D& GetOffset() const;
	const VkExtent2D& GetExtent() const;

	void SetExtent(const VkExtent2D& extent);
	void SetOffset(const VkOffset2D& offset);

	const float GetAspectRatio() const;

	void CmdUse(VkCommandBuffer commandBuffer) const;

private:
	// perchance change to glm::vec2
	VkOffset2D _offset;
	VkExtent2D _extent;
};

extern KtViewport WindowViewport;