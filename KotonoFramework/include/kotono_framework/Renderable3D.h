#pragma once
#include <vulkan/vulkan.h>
class KtRenderable3D
{
public:
	virtual ~KtRenderable3D() = default;

	virtual void CmdBind(VkCommandBuffer commandBuffer) const = 0;
	virtual void CmdDraw(VkCommandBuffer commandBuffer, const uint32_t instanceCount, const uint32_t firstInstance) const = 0;
};

