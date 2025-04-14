#include "Renderer3D.h"
#include "Framework.h"
#include "ShaderManager.h"
#include "Path.h"
#include "log.h"
#include "Renderer.h"

void KtRenderer3D::Init()
{
	const auto path = Framework.GetPath().GetFrameworkPath() / R"(shaders\wireframe3D.ktshader)";
	_wireframeShader = Framework.GetShaderManager().Create(path);
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

void KtRenderer3D::AddToWireframeQueue(const KtAddToWireframeQueue3DArgs& args)
{
	_wireframeQueueData[Framework.GetRenderer().GetGameThreadFrame()]
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
	CmdDrawRenderQueue(commandBuffer, culledData, currentFrame);

	CmdDrawWireframeQueue(commandBuffer, _wireframeQueueData[currentFrame], currentFrame);
}

void KtRenderer3D::CmdDrawWireframeQueue(VkCommandBuffer commandBuffer, const KtWireframeQueue3DData& wireframeQueueData, const uint32_t currentFrame) const
{
	std::vector<KtObjectData3D> objectBufferData;
	for (auto& [model, modelData] : wireframeQueueData.Models)
	{
		for (auto& [viewport, viewportData] : modelData.Viewports)
		{
			objectBufferData.insert(objectBufferData.end(),
				viewportData.ObjectDatas.begin(), viewportData.ObjectDatas.end()
			);
		}
	}
	if (objectBufferData.empty())
	{
		return;
	}

	// NOT A CMD, UPDATE ONCE PER FRAME //
	if (auto* binding = _wireframeShader->GetDescriptorSetLayoutBinding("objectBuffer"))
	{
		_wireframeShader->UpdateDescriptorSetLayoutBindingMemberCount(*binding, objectBufferData.size(), currentFrame);
		_wireframeShader->UpdateDescriptorSetLayoutBindingBuffer(*binding, objectBufferData.data(), currentFrame);
	}
	if (auto* binding = _wireframeShader->GetDescriptorSetLayoutBinding("cameraData"))
	{
		_wireframeShader->UpdateDescriptorSetLayoutBindingBuffer(*binding, (void*)(&_uniformData), currentFrame);
	}
	// -------------------------------- //

	_wireframeShader->CmdBind(commandBuffer);
	_wireframeShader->CmdBindDescriptorSets(commandBuffer, currentFrame);

	uint32_t instanceIndex = 0;
	for (auto& [model, modelData] : _wireframeQueueData[currentFrame].Models)
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

void KtRenderer3D::CmdDrawRenderQueue(VkCommandBuffer commandBuffer, const KtRenderQueue3DData& renderQueueData, const uint32_t currentFrame) const
{
	for (auto& [shader, shaderData] : renderQueueData.Shaders)
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
	_wireframeQueueData[currentFrame] = {};
}