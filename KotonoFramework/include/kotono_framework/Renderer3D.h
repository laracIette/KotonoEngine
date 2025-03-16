#pragma once
#include <vector>
#include <unordered_map>
#include "ObjectData3D.h"
#include "Shader3D.h"
#include "Model.h"
class KtRenderer3D
{
public:
	void Init();
	void Cleanup();

	void AddToRenderQueue(KtShader3D* shader, KtModel* model, const KtObjectData3D& objectData);
	void SetUniformData(const KtUniformData3D& uniformData);

	void CmdDraw(VkCommandBuffer commandBuffer, const uint32_t currentFrame) const;
	
	void ClearRenderQueue();

private:
	struct RenderQueue3DModelData
	{
		std::vector<KtObjectData3D> ObjectDatas;
	};
	struct RenderQueue3DShaderData
	{
		std::unordered_map<KtModel*, RenderQueue3DModelData> Models;
	};
	struct RenderQueue3DData
	{
		std::unordered_map<KtShader3D*, RenderQueue3DShaderData> Shaders;
	};

	RenderQueue3DData _renderQueue3DData;
	KtUniformData3D _uniformData3D;
};

