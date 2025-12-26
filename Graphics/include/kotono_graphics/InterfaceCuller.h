#pragma once
#include <cstdint>
template <typename T>
class KtPool;
class UInterfaceProxy;
class KtInterfaceCuller final
{
private:
	using Proxy = UInterfaceProxy;
	using ProxiesPool = KtPool<Proxy*>;

public:
	ProxiesPool ComputeCulling(ProxiesPool renderQueueData, const uint32_t frameIndex) const;

private:
	ProxiesPool ComputeNullCulling(const ProxiesPool& renderQueueData, const uint32_t frameIndex) const;
	ProxiesPool ComputeScreenCulling(const ProxiesPool& renderQueueData, const uint32_t frameIndex) const;
};

