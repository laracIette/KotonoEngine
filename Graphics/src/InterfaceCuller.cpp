#include "InterfaceCuller.h"
#include <kotono_common/log.h>
#include "InterfaceProxy.h"
#include <kotono_common/Pool.h>

#define KT_LOG_IMPORTANCE_LEVEL_NULLPTR ELogImportanceLevel::High

KtInterfaceCuller::ProxiesPool KtInterfaceCuller::ComputeCulling(ProxiesPool proxies, const u32 frameIndex) const
{
	proxies = ComputeNullCulling(proxies, frameIndex);
	proxies = ComputeScreenCulling(proxies, frameIndex);
	return proxies;
}

KtInterfaceCuller::ProxiesPool KtInterfaceCuller::ComputeNullCulling(const ProxiesPool& proxies, const u32 frameIndex) const
{
	ProxiesPool culledData{};

	for (Proxy* proxy : proxies)
	{
		if (!proxy)
		{
			KT_LOG(KT_LOG_IMPORTANCE_LEVEL_NULLPTR, "Graphics", "proxy is nullptr");
			continue;
		}

		if (!proxy->frameDatas_[frameIndex].data.shader)
		{
			KT_LOG(KT_LOG_IMPORTANCE_LEVEL_NULLPTR, "Graphics", "shader is nullptr");
			continue;
		}

		if (!proxy->frameDatas_[frameIndex].data.renderable)
		{
			KT_LOG(KT_LOG_IMPORTANCE_LEVEL_NULLPTR, "Graphics", "renderable is nullptr");
			continue;
		}

		culledData.Add(proxy);
	}

	return culledData;
}

KtInterfaceCuller::ProxiesPool KtInterfaceCuller::ComputeScreenCulling(const ProxiesPool& proxies, const u32 frameIndex) const
{
	ProxiesPool culledData{};

	for (Proxy* proxy : proxies)
	{
		// check out of bounds
		culledData.Add(proxy);
	}

	return culledData;
}
