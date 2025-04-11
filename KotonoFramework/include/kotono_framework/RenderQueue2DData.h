#pragma once
#include <vector>
#include <unordered_map>
#include <map>
#include "Shader.h"
#include "Viewport.h"
#include "ObjectData2D.h"
struct KtRenderQueue2DData
{
	struct RenderQueue2DLayerData
	{
		std::vector<KtObjectData2D> ObjectDatas;
	};
	struct RenderQueue2DViewportData
	{
		std::map<int32_t, RenderQueue2DLayerData> Layers;
	};
	struct RenderQueue2DShaderData
	{
		std::unordered_map<KtViewport*, RenderQueue2DViewportData> Viewports;
	};

	std::unordered_map<KtShader*, RenderQueue2DShaderData> Shaders;
};

