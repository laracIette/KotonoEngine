#include "Renderer3D.h"
#include "Framework.h"
#include "Context.h"
#include "Renderer.h"
#include "Culler3D.h"
#include "Renderable3D.h"
#include "Viewport.h"
#include "Shader.h"
#include "Renderable3DProxy.h"
#include "Collection.h"
#include "log.h"
#include "vk_utils.h"

void KtRenderer3D::Init()
{
	CreateStaticCommandBuffers();
	CreateDynamicCommandBuffers();
	isStaticCommandBufferDirty_.fill(true);
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

void KtRenderer3D::CreateStaticCommandBuffers()
{
	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = Framework.GetContext().GetCommandPool();
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_SECONDARY;
	allocInfo.commandBufferCount = static_cast<uint32_t>(staticCommandBuffers_.size());

	VK_CHECK_THROW(
		vkAllocateCommandBuffers(Framework.GetContext().GetDevice(), &allocInfo, staticCommandBuffers_.data()),
		"failed to allocate command buffers!"
	);
}

void KtRenderer3D::CreateDynamicCommandBuffers()
{
	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = Framework.GetContext().GetCommandPool();
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_SECONDARY;
	allocInfo.commandBufferCount = static_cast<uint32_t>(dynamicCommandBuffers_.size());

	VK_CHECK_THROW(
		vkAllocateCommandBuffers(Framework.GetContext().GetDevice(), &allocInfo, dynamicCommandBuffers_.data()),
		"failed to allocate command buffers!"
	);
}

void KtRenderer3D::RecordStaticCommandBuffer(const uint32_t currentFrame)
{
	VkCommandBuffer commandBuffer = staticCommandBuffers_[currentFrame];
	BeginCommandBuffer(commandBuffer, currentFrame);
	CmdDrawRenderQueue(commandBuffer, staticRenderQueueData_, currentFrame);
	EndCommandBuffer(commandBuffer);
}

void KtRenderer3D::RecordDynamicCommandBuffer(const uint32_t currentFrame)
{
	VkCommandBuffer commandBuffer = dynamicCommandBuffers_[currentFrame];
	BeginCommandBuffer(commandBuffer, currentFrame);
	CmdDrawRenderQueue(commandBuffer, renderQueueData_[currentFrame], currentFrame);
	EndCommandBuffer(commandBuffer);
}

void KtRenderer3D::BeginCommandBuffer(VkCommandBuffer commandBuffer, const uint32_t currentFrame)
{
	vkResetCommandBuffer(commandBuffer, 0);

	VkCommandBufferInheritanceInfo inheritanceInfo{};
	inheritanceInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
	inheritanceInfo.renderPass = Framework.GetRenderer().GetRenderPass();
	inheritanceInfo.subpass = 0;
	inheritanceInfo.framebuffer = Framework.GetRenderer().GetFramebuffer(currentFrame);

	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;
	beginInfo.pInheritanceInfo = &inheritanceInfo;

	VK_CHECK_THROW(
		vkBeginCommandBuffer(commandBuffer, &beginInfo),
		"failed to begin recording command buffer!"
	);
}

void KtRenderer3D::EndCommandBuffer(VkCommandBuffer commandBuffer)
{
	VK_CHECK_THROW(
		vkEndCommandBuffer(commandBuffer),
		"failed to record command buffer!"
	);
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

	const KtCuller3D culler{};
	const auto culledData = culler.ComputeCulling(renderQueueData_[currentFrame]);

	UpdateDescriptorSets(culledData, currentFrame);

	instanceIndices_[currentFrame] = {};
	stats_[currentFrame] = {};

	if (isStaticCommandBufferDirty_[currentFrame])
	{
		isStaticCommandBufferDirty_[currentFrame] = false;
		RecordStaticCommandBuffer(currentFrame);
	}
	RecordDynamicCommandBuffer(currentFrame);

	const std::array<VkCommandBuffer, 2> commandBuffers =
	{
		staticCommandBuffers_[currentFrame],
		dynamicCommandBuffers_[currentFrame]
	};

	vkCmdExecuteCommands(commandBuffer, static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
}

void KtRenderer3D::UpdateDescriptorSets(const KtRenderQueue3DData& renderQueueData, const uint32_t currentFrame)
{
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

		if (auto* binding = shader->GetDescriptorSetLayoutBinding("objectBuffer"))
		{
			shader->UpdateDescriptorSetLayoutBindingBufferMemberCount(*binding, objectBufferData.size(), currentFrame);
			shader->UpdateDescriptorSetLayoutBindingBuffer(*binding, objectBufferData.data(), currentFrame);
		}
		if (auto* binding = shader->GetDescriptorSetLayoutBinding("cameraData"))
		{
			shader->UpdateDescriptorSetLayoutBindingBuffer(*binding, (void*)(&uniformData_), currentFrame);
		}
	}
}

void KtRenderer3D::CmdDrawRenderQueue(VkCommandBuffer commandBuffer, const KtRenderQueue3DData& renderQueueData, const uint32_t currentFrame)
{
	KtViewport* currentViewport = nullptr;

	for (const auto& [shader, shaderData] : renderQueueData.shaders)
	{
		shader->CmdBind(commandBuffer);
		shader->CmdBindDescriptorSets(commandBuffer, currentFrame);

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

				renderable->CmdDraw(commandBuffer, instanceCount, instanceIndices_[currentFrame][shader]);
				instanceIndices_[currentFrame][shader] += instanceCount;

				stats_[currentFrame].drawCalls++;
			}
		}
	}
}\