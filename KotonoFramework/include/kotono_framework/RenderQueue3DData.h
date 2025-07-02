#pragma once
#include <unordered_map>
#include "ObjectData3D.h"

class KtShader;
class KtRenderable3D;
class KtViewport;
struct KtRenderable3DProxy;

struct KtRenderQueue3DData
{
	struct RenderQueue3DViewportData
	{
		std::unordered_map<KtRenderable3DProxy*, KtObjectData3D> ObjectDatas;
	};
	struct RenderQueue3DRenderableData
	{
		std::unordered_map<KtViewport*, RenderQueue3DViewportData> Viewports;
	};
	struct RenderQueue3DShaderData
	{
		std::unordered_map<KtRenderable3D*, RenderQueue3DRenderableData> Renderables;
	};

	std::unordered_map<KtShader*, RenderQueue3DShaderData> Shaders;
};
