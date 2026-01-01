#pragma once
#include <vulkan/vulkan_core.h>

class KtSceneRenderable
{
public:
	virtual ~KtSceneRenderable() = default;

	virtual void CmdBind(VkCommandBuffer commandBuffer) const = 0;
	virtual void CmdDraw(VkCommandBuffer commandBuffer, const u32 frameIndex) const = 0;

	virtual void UpdateIndirectBuffer(const u32 firstInstance, const u32 instanceCount, const u32 frameIndex) const = 0;
};

