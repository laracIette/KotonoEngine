#include "Culler2D.h"
#include "log.h"
#include "Renderable2DProxy.h"

#define KT_LOG_IMPORTANCE_LEVEL_NULLPTR KT_LOG_IMPORTANCE_LEVEL_HIGH

const KtCuller2D::ProxiesUnorderedSet KtCuller2D::ComputeCulling(ProxiesUnorderedSet proxies) const
{
	proxies = ComputeNullCulling(proxies);
	proxies = ComputeScreenCulling(proxies);
	return proxies;
}

const KtCuller2D::ProxiesUnorderedSet KtCuller2D::ComputeNullCulling(const ProxiesUnorderedSet& proxies) const
{
	ProxiesUnorderedSet culledData{};

	for (auto* proxy : proxies)
	{
		if (!proxy->shader)
		{
			KT_LOG_KF(KT_LOG_IMPORTANCE_LEVEL_NULLPTR, "KtCuller2D::ComputeNullCulling(): shader is nullptr");
			continue;
		}

		if (!proxy->renderable)
		{
			KT_LOG_KF(KT_LOG_IMPORTANCE_LEVEL_NULLPTR, "KtCuller2D::ComputeNullCulling(): renderable is nullptr");
			continue;
		}

		if (!proxy->viewport)
		{
			KT_LOG_KF(KT_LOG_IMPORTANCE_LEVEL_NULLPTR, "KtCuller2D::ComputeNullCulling(): viewport is nullptr");
			continue;
		}

		culledData.insert(proxy);
	}

	return culledData;
}

const KtCuller2D::ProxiesUnorderedSet KtCuller2D::ComputeScreenCulling(const ProxiesUnorderedSet& proxies) const
{
	ProxiesUnorderedSet culledData{};

	for (auto* proxy : proxies)
	{
		// check out of bounds
		culledData.insert(proxy);
	}

	return culledData;
}
