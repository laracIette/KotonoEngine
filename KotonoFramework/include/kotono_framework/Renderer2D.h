#pragma once
#include <vector>
#include <unordered_map>
#include "ObjectData2D.h"
#include "Shader2D.h"
class KtRenderer2D
{
public:
	void Init();
	void Cleanup() const;

	void AddToRenderQueue(KtShader2D* shader, const KtObjectData2D& objectData);
	void SetUniformData(const KtUniformData2D& uniformData);

	void CmdDraw(VkCommandBuffer commandBuffer, const uint32_t currentFrame);

	void ClearRenderQueue();

private:
	struct RenderQueue2DModelData
	{
		std::vector<KtObjectData2D> ObjectDatas;
	};
	struct RenderQueue2DData
	{
		std::unordered_map<KtShader2D*, RenderQueue2DModelData> Shaders;
	};

	RenderQueue2DData _renderQueue2DData;
	KtUniformData2D _uniformData2D;

	KtAllocatedBuffer _vertexBuffer;
	KtAllocatedBuffer _indexBuffer;

	void CreateVertexBuffer();
	void CreateIndexBuffer();
};

