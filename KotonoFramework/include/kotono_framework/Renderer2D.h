#pragma once
#include <vector>
#include <unordered_map>
#include "AddToRenderQueue2DArgs.h"
class KtRenderer2D
{
public:
	void Init();
	void Cleanup() const;

	void AddToRenderQueue(const KtAddToRenderQueue2DArgs& args);
	void SetUniformData(const KtUniformData2D& uniformData);

	void CmdDraw(VkCommandBuffer commandBuffer, const uint32_t currentFrame) const;

	void Reset();

private:
	struct RenderQueue2DViewportData
	{
		std::vector<KtObjectData2D> ObjectDatas;
	};
	struct RenderQueue2DShaderData
	{
		std::unordered_map<KtViewport*, RenderQueue2DViewportData> Viewports;
	};
	struct RenderQueue2DData
	{
		std::unordered_map<KtShader2D*, RenderQueue2DShaderData> Shaders;
	};

	RenderQueue2DData _renderQueue2DData;
	KtUniformData2D _uniformData;

	KtAllocatedBuffer _vertexBuffer;
	KtAllocatedBuffer _indexBuffer;

	void CreateVertexBuffer();
	void CreateIndexBuffer();

	void CmdBindBuffers(VkCommandBuffer commandBuffer) const;
};

