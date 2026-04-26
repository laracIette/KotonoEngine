#pragma once
#include <kotono_common/types.h>
template <typename T>
class UPool;
class UInterfaceProxy;
class KtInterfaceCuller final
{
private:
	using Proxy = UInterfaceProxy;
	using ProxiesPool = UPool<Proxy*>;

public:
	ProxiesPool ComputeCulling(ProxiesPool renderQueueData, const u32 frameIndex) const;

private:
	ProxiesPool ComputeNullCulling(const ProxiesPool& renderQueueData, const u32 frameIndex) const;
	ProxiesPool ComputeScreenCulling(const ProxiesPool& renderQueueData, const u32 frameIndex) const;
};

