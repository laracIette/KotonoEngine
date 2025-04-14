#pragma once
#include <vector>
#include "ModelWireframe.h"
#include "Viewport.h"
#include "ObjectData3D.h"
#include <unordered_map>
struct KtWireframeQueue3DData
{
	struct WireframeQueue3DViewportData
	{
		std::vector<KtObjectData3D> ObjectDatas;
	};
	struct WireframeQueue3DModelData
	{
		std::unordered_map<KtViewport*, WireframeQueue3DViewportData> Viewports;
	};
	std::unordered_map<KtModelWireframe*, WireframeQueue3DModelData> Models;
};