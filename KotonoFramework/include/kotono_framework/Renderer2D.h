#pragma once
#include <vector>
#include <unordered_map>
#include "ObjectData2D.h"
#include "Shader2D.h"
#include "RenderLayer.h"
class KtRenderer2D
{
public:
	void Init();
	void CmdDraw(VkCommandBuffer commandBuffer, const uint32_t currentFrame);

private:
	struct RenderQueue2DModelData
	{
		std::vector<KtObjectData2D> ObjectDatas;
	};
	struct RenderQueue2DData
	{
		std::unordered_map<KtShader2D*, RenderQueue2DModelData> Shaders;
	};
	struct Renderer2DData
	{
		std::array<RenderQueue2DData, KT_RENDER_LAYER_COUNT> RenderQueues;
	};

	Renderer2DData _renderer2DData;
	KtUniformData2D _uniformData2D;

	KtAllocatedBuffer _vertexBuffer;
	KtAllocatedBuffer _indexBuffer;

	void CreateVertexBuffer();
	void CreateIndexBuffer();
};

