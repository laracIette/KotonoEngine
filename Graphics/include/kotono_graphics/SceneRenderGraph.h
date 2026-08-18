#pragma once
#include "DirectionalLightData.h"
#include "DrawData.h"
#include "PointLightData.h"
#include <vector>
struct USceneRenderGraph final
{
	std::vector<UDrawData> drawDatas;
	std::vector<UDirectionalLightData> directionalLightDatas;
	std::vector<UPointLightData> pointLightDatas;
};
