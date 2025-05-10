#pragma once
#include "RenderQueue2DData.h"
class KtCuller2D final
{
public:
	const KtRenderQueue2DData ComputeCulling(KtRenderQueue2DData renderQueueData) const;

private:
	const KtRenderQueue2DData ComputeNullCulling(const KtRenderQueue2DData& renderQueueData) const;
	const KtRenderQueue2DData ComputeScreenCulling(const KtRenderQueue2DData& renderQueueData) const;
};

