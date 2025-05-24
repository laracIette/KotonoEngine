#include "Culler2D.h"
#include "log.h"

const KtRenderQueue2DData KtCuller2D::ComputeCulling(KtRenderQueue2DData renderQueueData) const
{
	renderQueueData = ComputeNullCulling(renderQueueData);
	renderQueueData = ComputeScreenCulling(renderQueueData);
	return renderQueueData;
}

const KtRenderQueue2DData KtCuller2D::ComputeNullCulling(const KtRenderQueue2DData& renderQueueData) const
{
	KtRenderQueue2DData culledData{};
	for (const auto& [shader, shaderData] : renderQueueData.Shaders)
	{
		if (!shader)
		{
			KT_DEBUG_LOG(KT_LOG_IMPORTANCE_LEVEL_HIGH, "KtCuller2D::ComputeNullCulling(): shader is nullptr");
			continue;
		}
		
		for (const auto& [renderable, renderableData] : shaderData.Renderables)
		{
			if (!renderable)
			{
				KT_DEBUG_LOG(KT_LOG_IMPORTANCE_LEVEL_HIGH, "KtCuller2D::ComputeNullCulling(): renderable is nullptr");
				continue;
			}

			for (const auto& [viewport, viewportData] : renderableData.Viewports)
			{
				if (!viewport)
				{
					KT_DEBUG_LOG(KT_LOG_IMPORTANCE_LEVEL_HIGH, "KtCuller2D::ComputeNullCulling(): viewport is nullptr");
					continue;
				}

				culledData.Shaders[shader].Renderables[renderable].Viewports[viewport] = viewportData;
			}
		}
		
	}

	return culledData;
}

const KtRenderQueue2DData KtCuller2D::ComputeScreenCulling(const KtRenderQueue2DData& renderQueueData) const
{
	KtRenderQueue2DData culledData{};
	for (const auto& [shader, shaderData] : renderQueueData.Shaders)
	{
		for (const auto& [renderable, renderableData] : shaderData.Renderables)
		{
			for (const auto& [viewport, viewportData] : renderableData.Viewports)
			{
				for (const auto& [layer, layerData] : viewportData.Layers)
				{
					// check out of bounds

					culledData.Shaders[shader].Renderables[renderable].Viewports[viewport].Layers[layer] = layerData;
				}
			}
		}
	}
	return culledData;
}
