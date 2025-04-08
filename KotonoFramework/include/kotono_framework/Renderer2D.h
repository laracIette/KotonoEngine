#pragma once
#include <vector>
#include <unordered_map>
#include "UniformData2D.h"
#include "AddToRenderQueue2DArgs.h"
#include "RenderQueue2DData.h"
#include "Culler2D.h"
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
	std::array<KtRenderQueue2DData, KT_FRAMES_IN_FLIGHT> _renderQueueData;
	std::array<KtUniformData2D, KT_FRAMES_IN_FLIGHT> _uniformData;
	KtCuller2D _culler;

	KtAllocatedBuffer _vertexBuffer;
	KtAllocatedBuffer _indexBuffer;

	void CreateVertexBuffer();
	void CreateIndexBuffer();

	void CmdBindBuffers(VkCommandBuffer commandBuffer) const;
};

