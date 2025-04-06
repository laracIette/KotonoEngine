#include "Renderer3D.h"
#include "Framework.h"
#include "log.h"
#include "Renderer.h"

void KtRenderer3D::Init()
{
}

void KtRenderer3D::Cleanup()
{
}

void KtRenderer3D::AddToRenderQueue(const KtAddToRenderQueue3DArgs& args)
{
	_renderQueueData[Framework.GetRenderer().GetGameThreadFrame()]
		.Shaders[args.Shader]
		.Models[args.Model]
		.Viewports[args.Viewport]
		.ObjectDatas.push_back(args.ObjectData);
}

void KtRenderer3D::SetUniformData(const KtUniformData3D& uniformData)
{
	_uniformData[Framework.GetRenderer().GetGameThreadFrame()] = uniformData;
}

void KtRenderer3D::CmdDraw(VkCommandBuffer commandBuffer, const uint32_t currentFrame) const
{
	const auto culledData = _culler.ComputeCulling(_renderQueueData[currentFrame]);

	for (auto& [shader, shaderData] : culledData.Shaders)
	{
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
		// NOT A CMD, UPDATE ONCE PER FRAME //
		if (auto* binding = shader->GetDescriptorSetLayoutBinding("objectBuffer"))
		{
			shader->UpdateDescriptorSetLayoutBindingMemberCount(*binding, objectBufferData.size(), currentFrame);
			shader->UpdateDescriptorSetLayoutBindingBuffer(*binding, objectBufferData.data(), currentFrame);
		}
		if (auto* binding = shader->GetDescriptorSetLayoutBinding("cameraData"))
		{
			shader->UpdateDescriptorSetLayoutBindingBuffer(*binding, (void*)(&_uniformData), currentFrame);
		}
		// -------------------------------- //

		shader->CmdBind(commandBuffer);
		shader->CmdBindDescriptorSets(commandBuffer, currentFrame);

		uint32_t instanceIndex = 0;
		for (auto& [model, modelData] : shaderData.Models)
		{			
			model->CmdBind(commandBuffer);
			
			for (auto& [viewport, viewportData] : modelData.Viewports)
			{
				const uint32_t instanceCount = static_cast<uint32_t>(viewportData.ObjectDatas.size());

				viewport->CmdUse(commandBuffer);
				model->CmdDraw(commandBuffer, instanceCount, instanceIndex);

				instanceIndex += instanceCount;
			}
		}
	}
}

void KtRenderer3D::Reset(const uint32_t currentFrame)
{
	_uniformData[currentFrame] = {};
	_renderQueueData[currentFrame] = {};
}