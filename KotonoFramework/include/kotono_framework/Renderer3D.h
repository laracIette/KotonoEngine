#pragma once
#include <vector>
#include <unordered_map>
#include "ObjectData3D.h"
#include "Shader3D.h"
#include "RenderLayer.h"
#include "Model.h"
class KtRenderer3D
{
public:
	void ClearRenderQueue();

	template <KtRenderLayer Layer>
	void AddToRenderQueue(KtShader3D* shader, KtModel* model, const KtObjectData3D& objectData);
	void SetUniformData(const KtUniformData3D& uniformData3D);

	void CmdDraw(VkCommandBuffer commandBuffer, const uint32_t currentFrame) const;

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
	struct Renderer3DData
	{
		std::array<RenderQueue3DData, KT_RENDER_LAYER_COUNT> RenderQueues;
	};

	Renderer3DData _renderer3DData;
	KtUniformData3D _uniformData3D;
};

