#include "Renderer3D.h"
#include "Framework.h"
#include "Renderer.h"
#include "Culler3D.h"
#include "Renderable3D.h"
#include "Viewport.h"
#include "Shader.h"
#include "Renderable3DProxy.h"
#include "Collection.h"
#include "log.h"

void KtRenderer3D::Init()
{
}

void KtRenderer3D::Cleanup()
{
}

void KtRenderer3D::SetUniformData(const KtUniformData3D& uniformData)
{
	uniformData_[Framework.GetRenderer().GetGameThreadFrame()] = uniformData;
}

void KtRenderer3D::Register(KtRenderable3DProxy* proxy)
{
	proxys_.insert(proxy);
	for (size_t i = 0; i < KT_FRAMES_IN_FLIGHT; i++)
	{
		AddProxyToRenderQueue(proxy, static_cast<uint32_t>(i));
	}
}

void KtRenderer3D::Remove(KtRenderable3DProxy* proxy)
{
	proxys_.erase(proxy);
	for (size_t i = 0; i < KT_FRAMES_IN_FLIGHT; i++)
	{
		RemoveProxyFromRenderQueue(proxy, static_cast<uint32_t>(i));
	}
}

void KtRenderer3D::CmdDraw(VkCommandBuffer commandBuffer, const uint32_t currentFrame)
{
	UpdateProxys(currentFrame);
	const KtCuller3D culler{};
	const auto culledData = culler.ComputeCulling(renderQueueData_[currentFrame]);
	CmdDrawRenderQueue(commandBuffer, culledData, currentFrame);
}

void KtRenderer3D::UpdateProxys(const uint32_t currentFrame)
{
	auto proxys = KtCollection(proxys_);
	proxys.AddFilter([](const auto* proxy) { return proxy->isDirty; });
	for (auto* proxy : proxys)
	{
		for (size_t i = 0; i < KT_FRAMES_IN_FLIGHT; i++)
		{
			AddProxyToRenderQueue(proxy, static_cast<uint32_t>(i));
		}
	}
}

void KtRenderer3D::AddProxyToRenderQueue(KtRenderable3DProxy* proxy, const uint32_t currentFrame)
{
	renderQueueData_[currentFrame]
		.Shaders[proxy->shader]
		.Renderables[proxy->renderable]
		.Viewports[proxy->viewport]
		.ObjectDatas[proxy].ModelMatrix = proxy->modelMatrix;
}

void KtRenderer3D::RemoveProxyFromRenderQueue(KtRenderable3DProxy* proxy, const uint32_t currentFrame)
{
	renderQueueData_[currentFrame]
		.Shaders[proxy->shader]
		.Renderables[proxy->renderable]
		.Viewports[proxy->viewport]
		.ObjectDatas.erase(proxy);
}

void KtRenderer3D::CmdDrawRenderQueue(VkCommandBuffer commandBuffer, const KtRenderQueue3DData& renderQueueData, const uint32_t currentFrame)
{
	stats_[currentFrame] = {};
	KtViewport* currentViewport = nullptr;

	for (const auto& [shader, shaderData] : renderQueueData.Shaders)
	{
		std::vector<KtObjectData3D> objectBufferData; // todo: unordered set ?
		for (const auto& [renderable, renderableData] : shaderData.Renderables)
		{
			for (const auto& [viewport, viewportData] : renderableData.Viewports)
			{
				/*objectBufferData.insert(objectBufferData.end(),
					viewportData.ObjectDatas.begin(), viewportData.ObjectDatas.end()
				);*/
				for (const auto& [proxy, objectData] : viewportData.ObjectDatas)
				{
					objectBufferData.push_back(objectData); // todo: push matrix directly ?
				}
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
			shader->UpdateDescriptorSetLayoutBindingBuffer(*binding, (void*)(&uniformData_), currentFrame);
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

				if (currentViewport != viewport)
				{
					currentViewport = viewport;
					currentViewport->CmdUse(commandBuffer);
				}

				renderable->CmdDraw(commandBuffer, instanceCount, instanceIndex);
				instanceIndex += instanceCount;

				stats_[currentFrame].drawCalls++;
			}
		}
	}
}

void KtRenderer3D::Reset(const uint32_t currentFrame)
{
	/*uniformData_[currentFrame] = {};
	renderQueueData_[currentFrame] = {};*/
}