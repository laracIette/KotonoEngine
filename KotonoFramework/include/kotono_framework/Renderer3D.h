#pragma once
#include <vector>
#include <unordered_map>
#include "UniformData3D.h"
#include "AddToRenderQueue3DArgs.h"
class KtRenderer3D
{
public:
	void Init();
	void Cleanup();

	void AddToRenderQueue(const KtAddToRenderQueue3DArgs& args);
	void SetUniformData(const KtUniformData3D& uniformData);

	void CmdDraw(VkCommandBuffer commandBuffer, const uint32_t currentFrame) const;
	
	void Reset();

private:
	struct RenderQueue3DViewportData
	{
		std::vector<KtObjectData3D> ObjectDatas;
	};
	struct RenderQueue3DModelData
	{
		std::unordered_map<KtViewport*, RenderQueue3DViewportData> Viewports;
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
	KtUniformData3D _uniformData;
};

