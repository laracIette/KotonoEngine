#include "InterfaceCuller.h"
#include <kotono_common/log.h>
#include "InterfaceRenderableProxy.h"
#include <kotono_common/Pool.h>

#define KT_LOG_IMPORTANCE_LEVEL_NULLPTR KT_LOG_IMPORTANCE_LEVEL_HIGH

KtInterfaceCuller::ProxiesPool KtInterfaceCuller::ComputeCulling(ProxiesPool proxies) const
{
	proxies = ComputeNullCulling(proxies);
	proxies = ComputeScreenCulling(proxies);
	return proxies;
}

KtInterfaceCuller::ProxiesPool KtInterfaceCuller::ComputeNullCulling(const ProxiesPool& proxies) const
{
	ProxiesPool culledData{};

	for (auto* proxy : proxies)
	{
		if (!proxy)
		{
			KT_LOG(KT_LOG_IMPORTANCE_LEVEL_NULLPTR, "Graphics.KtInterfaceCuller::ComputeNullCulling()", "proxy is nullptr");
			continue;
		}

		if (!proxy->shader)
		{
			KT_LOG(KT_LOG_IMPORTANCE_LEVEL_NULLPTR, "Graphics.KtInterfaceCuller::ComputeNullCulling()", "shader is nullptr");
			continue;
		}

		if (!proxy->renderable)
		{
			KT_LOG(KT_LOG_IMPORTANCE_LEVEL_NULLPTR, "Graphics.KtInterfaceCuller::ComputeNullCulling()", "renderable is nullptr");
			continue;
		}

		culledData.Add(proxy);
	}

	return culledData;
}

KtInterfaceCuller::ProxiesPool KtInterfaceCuller::ComputeScreenCulling(const ProxiesPool& proxies) const
{
	ProxiesPool culledData{};

	for (auto* proxy : proxies)
	{
		// check out of bounds
		culledData.Add(proxy);
	}

	return culledData;
}
