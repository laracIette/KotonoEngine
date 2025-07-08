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

	void CmdDraw(VkCommandBuffer commandBuffer, const uint32_t frameIndex);

	void Reset(const uint32_t frameIndex);

private:
	FramesInFlightArray<VkCommandBuffer> commandBuffers_;

	FramesInFlightArray<KtRenderQueue2DData> renderQueueData_;
	FramesInFlightArray<KtUniformData2D> uniformDatas_;

	KtAllocatedBuffer vertexBuffer_;
	KtAllocatedBuffer indexBuffer_;

	void CreateVertexBuffer();
	void CreateIndexBuffer();

	void CmdBindVertexBuffer(VkCommandBuffer commandBuffer) const;
	void CmdBindIndexBuffer(VkCommandBuffer commandBuffer) const;

	void CreateCommandBuffers();
	void RecordCommandBuffer(const uint32_t frameIndex);
	void BeginCommandBuffer(VkCommandBuffer commandBuffer, const uint32_t frameIndex);
	void EndCommandBuffer(VkCommandBuffer commandBuffer);
	
	void UpdateDescriptorSets(const KtRenderQueue2DData& renderQueueData, const uint32_t frameIndex);
	void CmdDrawRenderQueue(VkCommandBuffer commandBuffer, const KtRenderQueue2DData& renderQueueData, const uint32_t frameIndex);
};

