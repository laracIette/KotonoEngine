#pragma once
#include "DirectionalLightData.h"
#include "PointLightData.h"
#include "SceneDrawData.h"
#include <vector>

struct USceneRenderGraph final
{
	std::vector<USceneDrawData> drawDatas;
	std::vector<UDirectionalLightData> directionalLightDatas;
	std::vector<UPointLightData> pointLightDatas;
};
