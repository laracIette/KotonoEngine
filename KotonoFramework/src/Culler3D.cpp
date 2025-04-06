#define KT_DEBUG_LOG_DISABLED
#include "Framework.h"
#include "Culler3D.h"
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
	for (const auto& [shader, shaderData] : renderQueueData.Shaders)
	{
		if (!shader)
		{
			KT_DEBUG_LOG("KtCuller3D::ComputeNullCulling(): shader is nullptr");
			continue;
		}

		for (const auto& [model, modelData] : shaderData.Models)
		{
			if (!model)
			{
				KT_DEBUG_LOG("KtCuller3D::ComputeNullCulling(): model is nullptr");
				continue;
			}

			for (const auto& [viewport, viewportData] : modelData.Viewports)
			{
				if (!viewport)
				{
					KT_DEBUG_LOG("KtCuller3D::ComputeNullCulling(): viewport is nullptr");
					continue;
				}

				culledData.Shaders[shader].Models[model].Viewports[viewport] = viewportData;
			}
		}
	}

	return culledData;
}

const KtRenderQueue3DData KtCuller3D::ComputeDistanceCulling(const KtRenderQueue3DData& renderQueueData) const
{
	KtRenderQueue3DData culledData{};
	for (const auto& [shader, shaderData] : renderQueueData.Shaders)
	{
		for (const auto& [model, modelData] : shaderData.Models)
		{
			for (const auto& [viewport, viewportData] : modelData.Viewports)
			{
				for (const auto& objectData : viewportData.ObjectDatas)
				{
					const glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 0.0f);
					const glm::vec3 objectPosition = glm::vec3(objectData.Model[3]);
					const float distance = glm::distance(cameraPosition, objectPosition);
					const float maxDistance = 1.0f;
					if (distance > maxDistance)
					{
						continue;
					}

					culledData.Shaders[shader].Models[model].Viewports[viewport].ObjectDatas.push_back(objectData);
				}
			}
		}
	}

	return culledData;
}
