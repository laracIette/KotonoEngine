#include "Renderer3D.h"
#include "log.h"

void KtRenderer3D::Init()
{
}

void KtRenderer3D::Cleanup()
{
}

void KtRenderer3D::AddToRenderQueue(KtShader3D* shader, KtModel* model, const KtObjectData3D& objectData)
{
	_renderQueue3DData.Shaders[shader].Models[model].ObjectDatas.push_back(objectData);
}

void KtRenderer3D::SetUniformData(const KtUniformData3D& uniformData)
{
	_uniformData3D = uniformData;
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

		shader->UpdateUniformBuffer(_uniformData3D, currentFrame);

		std::vector<KtObjectData3D> objectBufferData;
		for (auto& [model, modelData] : shaderData.Models)
		{
			objectBufferData.insert(objectBufferData.end(), modelData.ObjectDatas.begin(), modelData.ObjectDatas.end());
		}
		shader->UpdateObjectBuffer(objectBufferData, currentFrame);

		shader->CmdBind(commandBuffer);
		shader->CmdBindDescriptorSets(commandBuffer, currentFrame);

		uint32_t instanceIndex = 0;

		for (auto& [model, modelData] : shaderData.Models)
		{
			const uint32_t instanceCount = static_cast<uint32_t>(modelData.ObjectDatas.size());

			if (!model)
			{
				KT_DEBUG_LOG("model is nullptr");
			}
			else if (instanceCount == 0)
			{
				KT_DEBUG_LOG("objectDatas is empty");
			}
			else
			{
				model->CmdBind(commandBuffer);
				model->CmdDraw(commandBuffer, instanceCount, instanceIndex);
			}

			instanceIndex += instanceCount;
		}
	}
}

void KtRenderer3D::ClearRenderQueue()
{
	_renderQueue3DData = {};
}