#pragma once
#include <vulkan/vulkan_core.h>
class KtViewport
{
public:
	const VkOffset2D& GetOffset() const;
	const VkExtent2D& GetExtent() const;

	void SetOffset(const VkOffset2D& offset);
	void SetExtent(const VkExtent2D& extent);

	void CmdUse(VkCommandBuffer commandBuffer) const;

private:
	VkOffset2D _offset;
	VkExtent2D _extent;
};

