#include "SceneCuller.h"
#include <kotono_common/log.h>
#include "SceneRenderableProxy.h"
#include <kotono_common/Pool.h>
#include <kotono_common/bitwise_utils.h>

#define KT_LOG_IMPORTANCE_LEVEL_NULLPTR ELogImportanceLevel::High

KtSceneCuller::KtSceneCuller(const KtSceneCullerField field) :
	field_(field)
{
}

KtSceneCuller::ProxiesPool KtSceneCuller::ComputeCulling(ProxiesPool proxies) const
{
	if (has_flag(field_, KT_SCENE_CULLER_FIELD_NULLPTR))
	{
		proxies = ComputeNullCulling(proxies);
	}
	if (has_flag(field_, KT_SCENE_CULLER_FIELD_DISTANCE))
	{
		proxies = ComputeDistanceCulling(proxies);
	}
	return proxies;
}

KtSceneCuller::ProxiesPool KtSceneCuller::ComputeNullCulling(const ProxiesPool& proxies) const
{
	ProxiesPool culledData{};

	for (auto* proxy : proxies)
	{
		if (!proxy)
		{
			KT_LOG(KT_LOG_IMPORTANCE_LEVEL_NULLPTR, "Graphics.KtSceneCuller::ComputeNullCulling()", "proxy is nullptr");
			continue;
		}

		if (!proxy->shader)
		{
			KT_LOG(KT_LOG_IMPORTANCE_LEVEL_NULLPTR, "GraphicsKtSceneCuller::ComputeNullCulling()", "shader is nullptr");
			continue;
		}

		if (!proxy->renderable)
		{
			KT_LOG(KT_LOG_IMPORTANCE_LEVEL_NULLPTR, "GraphicsKtSceneCuller::ComputeNullCulling()", "renderable is nullptr");
			continue;
		}

		culledData.Add(proxy);
	}

	return culledData;
}

KtSceneCuller::ProxiesPool KtSceneCuller::ComputeDistanceCulling(const ProxiesPool& proxies) const
{
	ProxiesPool culledData{};

	for (auto* proxy : proxies)
	{
		const glm::vec3 cameraPosition{ 0.0f, 0.0f, 0.0f };
		const glm::vec3 objectPosition{ glm::vec3(proxy->objectData.modelMatrix[3]) };
		const float distance{ glm::distance(cameraPosition, objectPosition) };
		static constexpr float maxDistance{ 10000.0f };
		if (distance > maxDistance)
		{
			continue;
		}

		culledData.Add(proxy);
	}

	return culledData;
}
