#include "Culler3D.h"
#include "log.h"
#include "Renderable3DProxy.h"

#define KT_LOG_IMPORTANCE_LEVEL_NULLPTR KT_LOG_IMPORTANCE_LEVEL_HIGH

const KtCuller3D::ProxiesUnorderedSet KtCuller3D::ComputeCulling(ProxiesUnorderedSet proxies, const KtCuller3DField field) const
{
	if ((field & KT_CULLER_3D_FIELD_NULLPTR) == KT_CULLER_3D_FIELD_NULLPTR)
	{
		proxies = ComputeNullCulling(proxies);
	}
	if ((field & KT_CULLER_3D_FIELD_DISTANCE) == KT_CULLER_3D_FIELD_DISTANCE)
	{
		proxies = ComputeDistanceCulling(proxies);
	}
	return proxies;
}

const KtCuller3D::ProxiesUnorderedSet KtCuller3D::ComputeNullCulling(const ProxiesUnorderedSet& proxies) const
{
	ProxiesUnorderedSet culledData{};

	for (auto* proxy : proxies)
	{
		if (!proxy->shader)
		{
			KT_LOG_KF(KT_LOG_IMPORTANCE_LEVEL_NULLPTR, "KtCuller3D::ComputeNullCulling(): shader is nullptr");
			continue;
		}

		if (!proxy->renderable)
		{
			KT_LOG_KF(KT_LOG_IMPORTANCE_LEVEL_NULLPTR, "KtCuller3D::ComputeNullCulling(): renderable is nullptr");
			continue;
		}

		if (!proxy->viewport)
		{
			KT_LOG_KF(KT_LOG_IMPORTANCE_LEVEL_NULLPTR, "KtCuller3D::ComputeNullCulling(): viewport is nullptr");
			continue;
		}

		culledData.insert(proxy);
	}

	return culledData;
}

const KtCuller3D::ProxiesUnorderedSet KtCuller3D::ComputeDistanceCulling(const ProxiesUnorderedSet& proxies) const
{
	ProxiesUnorderedSet culledData{};

	for (auto* proxy : proxies)
	{
		const glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 0.0f);
		const glm::vec3 objectPosition = glm::vec3(proxy->objectData.ModelMatrix[3]);
		const float distance = glm::distance(cameraPosition, objectPosition);
		constexpr float maxDistance = 10.0f;
		if (distance > maxDistance)
		{
			continue;
		}

		culledData.insert(proxy);
	}

	return culledData;
}
