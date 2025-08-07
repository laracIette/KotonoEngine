#pragma once
#include "Pool.h"
struct KtRenderable2DProxy;
class KtCuller2D final
{
private:
	using ProxiesPool = KtPool<KtRenderable2DProxy*>;

public:
	ProxiesPool ComputeCulling(ProxiesPool renderQueueData) const;

private:
	ProxiesPool ComputeNullCulling(const ProxiesPool& renderQueueData) const;
	ProxiesPool ComputeScreenCulling(const ProxiesPool& renderQueueData) const;
};

