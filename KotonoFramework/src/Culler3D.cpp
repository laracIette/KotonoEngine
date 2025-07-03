#include "Culler3D.h"
#include "Framework.h"
#include "log.h"

const KtRenderQueue3DData KtCuller3D::ComputeCulling(KtRenderQueue3DData renderQueueData) const
{
	renderQueueData = ComputeNullCulling(renderQueueData);
	renderQueueData = ComputeDistanceCulling(renderQueueData);
	return renderQueueData;
}

const KtRenderQueue3DData KtCuller3D::ComputeNullCulling(const KtRenderQueue3DData& renderQueueData) const
{
	KtRenderQueue3DData culledData{};
	for (const auto& [shader, shaderData] : renderQueueData.shaders)
	{
		if (!shader)
		{
			KT_LOG_KF(KT_LOG_IMPORTANCE_LEVEL_HIGH, "KtCuller3D::ComputeNullCulling(): shader is nullptr");
			continue;
		}

		for (const auto& [renderable, renderableData] : shaderData.renderables)
		{
			if (!renderable)
			{
				KT_LOG_KF(KT_LOG_IMPORTANCE_LEVEL_HIGH, "KtCuller3D::ComputeNullCulling(): renderable is nullptr");
				continue;
			}

			for (const auto& [viewport, viewportData] : renderableData.viewports)
			{
				if (!viewport)
				{
					KT_LOG_KF(KT_LOG_IMPORTANCE_LEVEL_HIGH, "KtCuller3D::ComputeNullCulling(): viewport is nullptr");
					continue;
				}

				culledData.shaders[shader].renderables[renderable].viewports[viewport] = viewportData;
			}
		}
	}

	return culledData;
}

const KtRenderQueue3DData KtCuller3D::ComputeDistanceCulling(const KtRenderQueue3DData& renderQueueData) const
{
	KtRenderQueue3DData culledData{};
	for (const auto& [shader, shaderData] : renderQueueData.shaders)
	{
		for (const auto& [renderable, renderableData] : shaderData.renderables)
		{
			for (const auto& [viewport, viewportData] : renderableData.viewports)
			{
				for (const auto& [proxy, objectData] : viewportData.objectDatas)
				{
					const glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 0.0f);
					const glm::vec3 objectPosition = glm::vec3(objectData.ModelMatrix[3]);
					const float distance = glm::distance(cameraPosition, objectPosition);
					constexpr float maxDistance = 10.0f;
					if (distance > maxDistance)
					{
						continue;
					}

					culledData.shaders[shader].renderables[renderable].viewports[viewport].objectDatas[proxy] = objectData;
				}
			}
		}
	}

	return culledData;
}
