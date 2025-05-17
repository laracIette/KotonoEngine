#pragma once
#include "AddToRenderQueue3DArgs.h"
#include "UniformData3D.h"
#include "RenderQueue3DData.h"
#include "frames_in_flight.h"
#include "AllocatedBuffer.h"
#include <vulkan/vulkan_core.h>
#include <array>
class KtRenderer3D final
{
public:
	void Init();
	void Cleanup();

	void AddToRenderQueue(const KtAddToRenderQueue3DArgs& args);
	void SetUniformData(const KtUniformData3D& uniformData);

	void CmdDraw(VkCommandBuffer commandBuffer, const uint32_t currentFrame) const;
	
	void Reset(const uint32_t currentFrame);

private:
	std::array<KtRenderQueue3DData, KT_FRAMES_IN_FLIGHT> renderQueueData_;
	std::array<KtUniformData3D, KT_FRAMES_IN_FLIGHT> uniformData_;

	void CmdDrawRenderQueue(VkCommandBuffer commandBuffer, const KtRenderQueue3DData& renderQueueData, const uint32_t currentFrame) const;
};

