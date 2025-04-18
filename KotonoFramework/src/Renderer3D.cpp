#include "Renderer3D.h"
#include "Framework.h"
#include "ShaderManager.h"
#include "Path.h"
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
		.Renderables[args.Renderable]
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
	CmdDrawRenderQueue(commandBuffer, culledData, currentFrame);
}

void KtRenderer3D::CmdDrawRenderQueue(VkCommandBuffer commandBuffer, const KtRenderQueue3DData& renderQueueData, const uint32_t currentFrame) const
{
	for (auto& [shader, shaderData] : renderQueueData.Shaders)
	{
		std::vector<KtObjectData3D> objectBufferData;
		for (auto& [renderable, renderableData] : shaderData.Renderables)
		{
			for (auto& [viewport, viewportData] : renderableData.Viewports)
			{
				objectBufferData.insert(objectBufferData.end(),
					viewportData.ObjectDatas.begin(), viewportData.ObjectDatas.end()
				);
			}
		}
		// NOT A CMD, UPDATE ONCE PER FRAME //
		if (auto* binding = shader->GetDescriptorSetLayoutBinding("objectBuffer"))
		{
			shader->UpdateDescriptorSetLayoutBindingBufferMemberCount(*binding, objectBufferData.size(), currentFrame);
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
		for (auto& [renderable, renderableData] : shaderData.Renderables)
		{
			renderable->CmdBind(commandBuffer);

			for (auto& [viewport, viewportData] : renderableData.Viewports)
			{
				const uint32_t instanceCount = static_cast<uint32_t>(viewportData.ObjectDatas.size());

				viewport->CmdUse(commandBuffer);
				renderable->CmdDraw(commandBuffer, instanceCount, instanceIndex);

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