#pragma once
#include "SceneCullerField.h"
template <typename T>
class KtPool;
class USceneProxy;
class KtSceneCuller final
{
private:
	// todo: make proxy pool element to store index and not copy pool every time
	using Proxy = USceneProxy;
	using ProxiesPool = KtPool<const Proxy*>;

public:
	KtSceneCuller(const KtSceneCullerField field);

	ProxiesPool ComputeCulling(ProxiesPool renderQueueData, const uint32_t frameIndex) const;

private:
	const KtSceneCullerField field_;

	ProxiesPool ComputeNullCulling(const ProxiesPool& renderQueueData, const uint32_t frameIndex) const;
	ProxiesPool ComputeDistanceCulling(const ProxiesPool& renderQueueData, const uint32_t frameIndex) const;
};

