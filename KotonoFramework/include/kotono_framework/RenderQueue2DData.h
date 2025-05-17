#pragma once
#include <vector>
#include <unordered_map>
#include <map>
#include "ObjectData2D.h"

class KtShader;
class KtRenderable2D;
class KtViewport;

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
	struct RenderQueue2DRenderableData
	{
		std::unordered_map<KtViewport*, RenderQueue2DViewportData> Viewports;
	};
	struct RenderQueue2DShaderData
	{
		std::unordered_map<KtRenderable2D*, RenderQueue2DRenderableData> Renderables;
	};

	std::unordered_map<KtShader*, RenderQueue2DShaderData> Shaders;
};

