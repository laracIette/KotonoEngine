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
	for (const auto& [shader, shaderData] : renderQueueData.shaders)
	{
		if (!shader)
		{
			KT_LOG_KF(KT_LOG_IMPORTANCE_LEVEL_HIGH, "KtCuller2D::ComputeNullCulling(): shader is nullptr");
			continue;
		}
		
		for (const auto& [renderable, renderableData] : shaderData.renderables)
		{
			if (!renderable)
			{
				KT_LOG_KF(KT_LOG_IMPORTANCE_LEVEL_HIGH, "KtCuller2D::ComputeNullCulling(): renderable is nullptr");
				continue;
			}

			for (const auto& [viewport, viewportData] : renderableData.viewports)
			{
				if (!viewport)
				{
					KT_LOG_KF(KT_LOG_IMPORTANCE_LEVEL_HIGH, "KtCuller2D::ComputeNullCulling(): viewport is nullptr");
					continue;
				}

				culledData.shaders[shader].renderables[renderable].viewports[viewport] = viewportData;
			}
		}
		
	}

	return culledData;
}

const KtRenderQueue2DData KtCuller2D::ComputeScreenCulling(const KtRenderQueue2DData& renderQueueData) const
{
	KtRenderQueue2DData culledData{};
	for (const auto& [shader, shaderData] : renderQueueData.shaders)
	{
		for (const auto& [renderable, renderableData] : shaderData.renderables)
		{
			for (const auto& [viewport, viewportData] : renderableData.viewports)
			{
				for (const auto& [layer, layerData] : viewportData.Layers)
				{
					// check out of bounds

					culledData.shaders[shader].renderables[renderable].viewports[viewport].Layers[layer] = layerData;
				}
			}
		}
	}
	return culledData;
}
