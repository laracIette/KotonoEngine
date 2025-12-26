#include "SceneCuller.h"
#include <kotono_common/log.h>
#include "SceneProxy.h"
#include <kotono_common/Pool.h>
#include <kotono_common/bitwise_utils.h>

#define KT_LOG_IMPORTANCE_LEVEL_NULLPTR ELogImportanceLevel::High

KtSceneCuller::KtSceneCuller(const KtSceneCullerField field) :
	field_(field)
{
}

KtSceneCuller::ProxiesPool KtSceneCuller::ComputeCulling(ProxiesPool proxies, const uint32_t frameIndex) const
{
	if (has_flag(field_, KT_SCENE_CULLER_FIELD_NULLPTR))
	{
		proxies = ComputeNullCulling(proxies, frameIndex);
	}
	if (has_flag(field_, KT_SCENE_CULLER_FIELD_DISTANCE))
	{
		proxies = ComputeDistanceCulling(proxies, frameIndex);
	}
	return proxies;
}

KtSceneCuller::ProxiesPool KtSceneCuller::ComputeNullCulling(const ProxiesPool& proxies, const uint32_t frameIndex) const
{
	ProxiesPool culledData{};

	for (Proxy* proxy : proxies)
	{
		if (!proxy)
		{
			KT_LOG(KT_LOG_IMPORTANCE_LEVEL_NULLPTR, "GraphicsKtSceneCuller::ComputeNullCulling()", "proxy is nullptr");
			continue;
		}

		if (!proxy->frameDatas_[frameIndex].shader)
		{
			KT_LOG(KT_LOG_IMPORTANCE_LEVEL_NULLPTR, "GraphicsKtSceneCuller::ComputeNullCulling()", "shader is nullptr");
			continue;
		}

		if (!proxy->frameDatas_[frameIndex].renderable)
		{
			KT_LOG(KT_LOG_IMPORTANCE_LEVEL_NULLPTR, "GraphicsKtSceneCuller::ComputeNullCulling()", "renderable is nullptr");
			continue;
		}

		culledData.Add(proxy);
	}

	return culledData;
}

KtSceneCuller::ProxiesPool KtSceneCuller::ComputeDistanceCulling(const ProxiesPool& proxies, const uint32_t frameIndex) const
{
	ProxiesPool culledData{};

	for (Proxy* proxy : proxies)
	{
		const glm::vec3 cameraPosition{ 0.0f, 0.0f, 0.0f };
		const glm::vec3 objectPosition{ glm::vec3(proxy->frameDatas_[frameIndex].objectData.modelMatrix[3]) };
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
