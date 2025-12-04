#pragma once
#include "Pool.h"
struct KtInterfaceRenderableProxy;
class KtInterfaceCuller final
{
private:
	using ProxiesPool = KtPool<KtInterfaceRenderableProxy*>;

public:
	ProxiesPool ComputeCulling(ProxiesPool renderQueueData) const;

private:
	ProxiesPool ComputeNullCulling(const ProxiesPool& renderQueueData) const;
	ProxiesPool ComputeScreenCulling(const ProxiesPool& renderQueueData) const;
};

