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
	KtCuller3D(const KtCuller3DField field) : field_(field) {}

	ProxiesPool ComputeCulling(ProxiesPool renderQueueData) const;

private:
	const KtCuller3DField field_;

	ProxiesPool ComputeNullCulling(const ProxiesPool& renderQueueData) const;
	ProxiesPool ComputeDistanceCulling(const ProxiesPool& renderQueueData) const;
};

