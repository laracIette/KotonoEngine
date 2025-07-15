#pragma once
#include "Pool.h"
struct KtRenderable2DProxy;
class KtCuller2D final
{
private:
	using ProxiesPool = KtPool<KtRenderable2DProxy*>;

public:
	const ProxiesPool ComputeCulling(ProxiesPool renderQueueData) const;

private:
	const ProxiesPool ComputeNullCulling(const ProxiesPool& renderQueueData) const;
	const ProxiesPool ComputeScreenCulling(const ProxiesPool& renderQueueData) const;
};

