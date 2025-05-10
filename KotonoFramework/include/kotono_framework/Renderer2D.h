#pragma once
#include "UniformData2D.h"
#include "AddToRenderQueue2DArgs.h"
#include "RenderQueue2DData.h"
class KtRenderer2D final
{
public:
	void Init();
	void Cleanup() const;

	void AddToRenderQueue(const KtAddToRenderQueue2DArgs& args);
	void SetUniformData(const KtUniformData2D& uniformData);

	void CmdDraw(VkCommandBuffer commandBuffer, const uint32_t currentFrame) const;

	void Reset(const uint32_t currentFrame);

private:
	std::array<KtRenderQueue2DData, KT_FRAMES_IN_FLIGHT> renderQueueData_;
	std::array<KtUniformData2D, KT_FRAMES_IN_FLIGHT> uniformData_;

	KtAllocatedBuffer vertexBuffer_;
	KtAllocatedBuffer indexBuffer_;

	void CreateVertexBuffer();
	void CreateIndexBuffer();

	void CmdDrawRenderQueue(VkCommandBuffer commandBuffer, const KtRenderQueue2DData& renderQueueData, const uint32_t currentFrame) const;

	void CmdBindVertexBuffer(VkCommandBuffer commandBuffer) const;
	void CmdBindIndexBuffer(VkCommandBuffer commandBuffer) const;
};

