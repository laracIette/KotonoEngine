#pragma once

#include "Containers/List.h"
#include "DirectionalLightData.h"
#include "PointLightData.h"
#include "SceneDrawData.h"

struct USceneRenderGraph final
{
	UList<USceneDrawData> drawDatas;
	UList<UDirectionalLightData> directionalLightDatas;
	UList<UPointLightData> pointLightDatas;
};
