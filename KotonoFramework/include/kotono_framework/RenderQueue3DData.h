#pragma once
#include <vector>
#include <unordered_map>
#include "Shader.h"
#include "Model.h"
#include "Viewport.h"
#include "ObjectData3D.h"
struct KtRenderQueue3DData
{
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

	std::unordered_map<KtShader*, RenderQueue3DShaderData> Shaders;
};
