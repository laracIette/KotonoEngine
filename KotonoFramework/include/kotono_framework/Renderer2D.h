#pragma once
#include "UniformData2D.h"
#include "AddToRenderQueue2DArgs.h"
#include "RenderQueue2DData.h"
#include "frames_in_flight.h"
#include "AllocatedBuffer.h"
#include <vulkan/vulkan_core.h>
#include <array>
class KtRenderer2D final
{
public:
	void Init();
	void Cleanup() const;

	void AddToRenderQueue(const KtAddToRenderQueue2DArgs& args);
	void SetUniformData(const KtUniformData2D& uniformData);

	void CmdDraw(VkCommandBuffer commandBuffer, const uint32_t currentFrame);

	void Reset(const uint32_t currentFrame);

private:
	std::array<VkCommandBuffer, KT_FRAMES_IN_FLIGHT> commandBuffers_;

	std::array<KtRenderQueue2DData, KT_FRAMES_IN_FLIGHT> renderQueueData_;
	std::array<KtUniformData2D, KT_FRAMES_IN_FLIGHT> uniformData_;

	KtAllocatedBuffer vertexBuffer_;
	KtAllocatedBuffer indexBuffer_;

	void CreateVertexBuffer();
	void CreateIndexBuffer();

	void CmdBindVertexBuffer(VkCommandBuffer commandBuffer) const;
	void CmdBindIndexBuffer(VkCommandBuffer commandBuffer) const;

	void CreateCommandBuffers();
	void RecordCommandBuffer(const uint32_t currentFrame);
	void BeginCommandBuffer(VkCommandBuffer commandBuffer, const uint32_t currentFrame);
	void EndCommandBuffer(VkCommandBuffer commandBuffer);
	
	void UpdateDescriptorSets(const KtRenderQueue2DData& renderQueueData, const uint32_t currentFrame);
	void CmdDrawRenderQueue(VkCommandBuffer commandBuffer, const KtRenderQueue2DData& renderQueueData, const uint32_t currentFrame);
};

