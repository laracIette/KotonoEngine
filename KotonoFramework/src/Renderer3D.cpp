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

void KtRenderer3D::UpdateProxys()
{
	auto proxys = KtCollection(proxys_.begin(), proxys_.end());
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
		.shaders[proxy->shader]
		.renderables[proxy->renderable]
		.viewports[proxy->viewport]
		.objectDatas[proxy].ModelMatrix = proxy->modelMatrix;
}

void KtRenderer3D::RemoveProxyFromRenderQueue(KtRenderable3DProxy* proxy, const uint32_t currentFrame)
{
	renderQueueData_[currentFrame]
		.shaders[proxy->shader]
		.renderables[proxy->renderable]
		.viewports[proxy->viewport]
		.objectDatas.erase(proxy);
}

void KtRenderer3D::CmdDraw(VkCommandBuffer commandBuffer, const uint32_t currentFrame)
{
	UpdateProxys();
#if true
	const KtCuller3D culler{};
	const auto culledData = culler.ComputeCulling(renderQueueData_[currentFrame]);
	CmdDrawRenderQueue(commandBuffer, culledData, currentFrame);
#else
	CmdDrawRenderQueue(commandBuffer, renderQueueData_[currentFrame], currentFrame);
#endif
}

void KtRenderer3D::CmdDrawRenderQueue(VkCommandBuffer commandBuffer, const KtRenderQueue3DData& renderQueueData, const uint32_t currentFrame)
{
	stats_[currentFrame] = {};
	KtViewport* currentViewport = nullptr;

	for (const auto& [shader, shaderData] : renderQueueData.shaders)
	{
		std::vector<KtObjectData3D> objectBufferData;
		for (const auto& [renderable, renderableData] : shaderData.renderables)
		{
			for (const auto& [viewport, viewportData] : renderableData.viewports)
			{
				for (const auto& [proxy, objectData] : viewportData.objectDatas)
				{
					objectBufferData.push_back(objectData); 
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
		for (auto& [renderable, renderableData] : shaderData.renderables)
		{
			renderable->CmdBind(commandBuffer);

			for (auto& [viewport, viewportData] : renderableData.viewports)
			{
				const uint32_t instanceCount = static_cast<uint32_t>(viewportData.objectDatas.size());

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