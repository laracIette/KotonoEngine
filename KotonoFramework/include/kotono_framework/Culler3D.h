#pragma once
#include "RenderQueue3DData.h"
class KtCuller3D final
{
public:
	const KtRenderQueue3DData ComputeCulling(KtRenderQueue3DData renderQueueData) const;

private:
	const KtRenderQueue3DData ComputeNullCulling(const KtRenderQueue3DData& renderQueueData) const;
	const KtRenderQueue3DData ComputeDistanceCulling(const KtRenderQueue3DData& renderQueueData) const;

	KtRenderQueue3DData renderQueueData_;
};

