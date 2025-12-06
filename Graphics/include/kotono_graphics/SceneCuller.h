#pragma once
#include "SceneCullerField.h"
template <typename T>
class KtPool;
struct KtSceneRenderableProxy;
class KtSceneCuller final
{
private:
	// todo: make proxy pool element to store index and not copy pool every time
	using ProxiesPool = KtPool<KtSceneRenderableProxy*>;

public:
	KtSceneCuller(const KtSceneCullerField field);

	ProxiesPool ComputeCulling(ProxiesPool renderQueueData) const;

private:
	const KtSceneCullerField field_;

	ProxiesPool ComputeNullCulling(const ProxiesPool& renderQueueData) const;
	ProxiesPool ComputeDistanceCulling(const ProxiesPool& renderQueueData) const;
};

