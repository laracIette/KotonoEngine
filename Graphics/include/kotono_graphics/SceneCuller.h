#pragma once
#include "SceneCullerField.h"
template <typename T>
class UPool;
class USceneProxy;
class KtSceneCuller final
{
private:
	// todo: make proxy pool element to store index and not copy pool every time
	using Proxy = USceneProxy;
	using ProxiesPool = UPool<Proxy*>;

public:
	KtSceneCuller(const KtSceneCullerField field);

	ProxiesPool ComputeCulling(ProxiesPool renderQueueData, const u32 frameIndex) const;

private:
	const KtSceneCullerField field_;

	ProxiesPool ComputeNullCulling(const ProxiesPool& renderQueueData, const u32 frameIndex) const;
	ProxiesPool ComputeDistanceCulling(const ProxiesPool& renderQueueData, const u32 frameIndex) const;
};

