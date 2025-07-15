#pragma once
#include "Pool.h"
#include "Culler3DField.h"
struct KtRenderable3DProxy;
class KtCuller3D final
{
private:
	// todo: make proxy pool element to store index and not copy pool every time
	using ProxiesPool = KtPool<KtRenderable3DProxy*>;

public:
	const ProxiesPool ComputeCulling(ProxiesPool renderQueueData, const KtCuller3DField field) const;

private:
	const ProxiesPool ComputeNullCulling(const ProxiesPool& renderQueueData) const;
	const ProxiesPool ComputeDistanceCulling(const ProxiesPool& renderQueueData) const;
};

