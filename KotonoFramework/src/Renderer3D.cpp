#include "Renderer3D.h"
#include "log.h"

void KtRenderer3D::Init()
{
}

void KtRenderer3D::Cleanup()
{
}

void KtRenderer3D::AddToRenderQueue(const KtAddToRenderQueue3DArgs& args)
{
	_renderQueue3DData
		.Shaders[args.Shader]
		.Models[args.Model]
		.Viewports[args.Viewport]
		.ObjectDatas.push_back(args.ObjectData);
}

void KtRenderer3D::SetUniformData(const KtUniformData3D& uniformData)
{
	_uniformData = uniformData;
}

void KtRenderer3D::CmdDraw(VkCommandBuffer commandBuffer, const uint32_t currentFrame) const
{
	for (auto& [shader, shaderData] : _renderQueue3DData.Shaders)
	{
		if (!shader)
		{
			KT_DEBUG_LOG("shader is nullptr");
			continue;
		}

		shader->CmdBind(commandBuffer);
		shader->CmdBindDescriptorSets(commandBuffer, currentFrame);

		std::vector<KtObjectData3D> objectBufferData;
		for (auto& [model, modelData] : shaderData.Models)
		{
			for (auto& [viewport, viewportData] : modelData.Viewports)
			{
				objectBufferData.insert(objectBufferData.end(),
					viewportData.ObjectDatas.begin(), viewportData.ObjectDatas.end()
				);
			}
		}
		shader->UpdateObjectBuffer(objectBufferData, currentFrame);
		shader->UpdateUniformBuffer(_uniformData, currentFrame);

		uint32_t instanceIndex = 0;
		for (auto& [model, modelData] : shaderData.Models)
		{
			if (!model)
			{
				KT_DEBUG_LOG("model is nullptr");
				for (auto& [viewport, viewportData] : modelData.Viewports)
				{
					instanceIndex += static_cast<uint32_t>(viewportData.ObjectDatas.size());
				}
				continue;
			}
			
			model->CmdBind(commandBuffer);
			
			for (auto& [viewport, viewportData] : modelData.Viewports)
			{
				const uint32_t instanceCount = static_cast<uint32_t>(viewportData.ObjectDatas.size());

				if (!viewport)
				{
					KT_DEBUG_LOG("viewport is nullptr");
				}
				else if (instanceCount == 0)
				{
					KT_DEBUG_LOG("model doesn't have any instance");
				}
				else
				{
					viewport->CmdUse(commandBuffer);
					model->CmdDraw(commandBuffer, instanceCount, instanceIndex);
				}

				instanceIndex += instanceCount;
			}
		}
	}
}

void KtRenderer3D::Reset()
{
	_uniformData = {};
	_renderQueue3DData = {};
}