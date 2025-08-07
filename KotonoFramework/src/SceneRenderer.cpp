#include "SceneRenderer.h"
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
#include <unordered_set>

#define KT_LOG_IMPORTANCE_LEVEL_PROXY KT_LOG_IMPORTANCE_LEVEL_HIGH

void KtSceneRenderer::Init()
{
	CreateStaticCommandBuffers();
	CreateDynamicCommandBuffers();
	isUniformBufferDirty_.fill(true);
	isStaticCommandBufferDirty_.fill(true);
	isDynamicCommandBufferDirty_.fill(true);
}

void KtSceneRenderer::Update(const uint32_t frameIndex)
{
	UpdateUniformData(frameIndex);
	UpdateStaticProxies(frameIndex);
	UpdateDynamicProxies(frameIndex);
}

void KtSceneRenderer::Cleanup()
{
}

void KtSceneRenderer::SetUniformData(const KtUniformData3D& uniformData)
{
	stagingUniformData_ = { uniformData, static_cast<uint32_t>(KT_FRAMES_IN_FLIGHT) };
}

void KtSceneRenderer::RegisterStatic(KtRenderable3DProxy* proxy)
{
	stagingStaticProxies_[proxy] = static_cast<int32_t>(KT_FRAMES_IN_FLIGHT);
}

void KtSceneRenderer::RegisterDynamic(KtRenderable3DProxy* proxy)
{
	stagingDynamicProxies_[proxy] = static_cast<int32_t>(KT_FRAMES_IN_FLIGHT);
}

void KtSceneRenderer::UnregisterStatic(KtRenderable3DProxy* proxy)
{
	stagingStaticProxies_[proxy] = -static_cast<int32_t>(KT_FRAMES_IN_FLIGHT);
}

void KtSceneRenderer::UnregisterDynamic(KtRenderable3DProxy* proxy)
{
	stagingDynamicProxies_[proxy] = -static_cast<int32_t>(KT_FRAMES_IN_FLIGHT);
}

void KtSceneRenderer::CreateStaticCommandBuffers()
{
	for (size_t i = 0; i < KT_FRAMES_IN_FLIGHT; ++i)
	{
		CreateStaticCommandBuffer(static_cast<uint32_t>(i));
	}
}

void KtSceneRenderer::CreateStaticCommandBuffer(const uint32_t frameIndex)
{
	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = Framework.GetRenderer().GetCommandPool(frameIndex);
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_SECONDARY;
	allocInfo.commandBufferCount = 1;

	VK_CHECK_THROW(
		vkAllocateCommandBuffers(Framework.GetContext().GetDevice(), &allocInfo, &staticCommandBuffers_[frameIndex]),
		"failed to allocate command buffers!"
	);
}

void KtSceneRenderer::CreateDynamicCommandBuffers()
{
	for (size_t i = 0; i < KT_FRAMES_IN_FLIGHT; ++i)
	{
		CreateDynamicCommandBuffer(static_cast<uint32_t>(i));
	}
}

void KtSceneRenderer::CreateDynamicCommandBuffer(const uint32_t frameIndex)
{
	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = Framework.GetRenderer().GetCommandPool(frameIndex);
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_SECONDARY;
	allocInfo.commandBufferCount = 1;

	VK_CHECK_THROW(
		vkAllocateCommandBuffers(Framework.GetContext().GetDevice(), &allocInfo, &dynamicCommandBuffers_[frameIndex]),
		"failed to allocate command buffers!"
	);
}

void KtSceneRenderer::RecordStaticCommandBuffer(const uint32_t frameIndex)
{
	SortProxies(staticProxies_[frameIndex]);
	VkCommandBuffer commandBuffer = staticCommandBuffers_[frameIndex];
	BeginCommandBuffer(commandBuffer, frameIndex);
	CmdDrawProxies(commandBuffer, staticProxies_[frameIndex], frameIndex);
	EndCommandBuffer(commandBuffer);
}

void KtSceneRenderer::RecordDynamicCommandBuffer(const uint32_t frameIndex)
{
	SortProxies(dynamicProxies_[frameIndex]);
	VkCommandBuffer commandBuffer = dynamicCommandBuffers_[frameIndex];
	BeginCommandBuffer(commandBuffer, frameIndex);
	CmdDrawProxies(commandBuffer, dynamicProxies_[frameIndex], frameIndex);
	EndCommandBuffer(commandBuffer);
}

void KtSceneRenderer::BeginCommandBuffer(VkCommandBuffer commandBuffer, const uint32_t frameIndex)
{
	vkResetCommandBuffer(commandBuffer, 0);

	VkCommandBufferInheritanceInfo inheritanceInfo{};
	inheritanceInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
	inheritanceInfo.renderPass = Framework.GetRenderer().GetRenderPass();
	inheritanceInfo.subpass = 0;
	inheritanceInfo.framebuffer = Framework.GetRenderer().GetFramebuffer(frameIndex);

	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;
	beginInfo.pInheritanceInfo = &inheritanceInfo;

	VK_CHECK_THROW(
		vkBeginCommandBuffer(commandBuffer, &beginInfo),
		"failed to begin recording command buffer!"
	);
}

void KtSceneRenderer::EndCommandBuffer(VkCommandBuffer commandBuffer)
{
	VK_CHECK_THROW(
		vkEndCommandBuffer(commandBuffer),
		"failed to record command buffer!"
	);
}

void KtSceneRenderer::UpdateUniformData(const uint32_t frameIndex)
{
	if (stagingUniformData_.second == 0)
	{
		return;
	}

	isUniformBufferDirty_[frameIndex] = true;
	uniformDatas_[frameIndex] = stagingUniformData_.first;
	--stagingUniformData_.second;
}

void KtSceneRenderer::UpdateStaticProxies(const uint32_t frameIndex)
{
	if (stagingStaticProxies_.empty())
	{
		return;
	}

	for (auto& [proxy, count] : stagingStaticProxies_)
	{
		if (count == 0)
		{
			continue;
		}

		isStaticCommandBufferDirty_[frameIndex] = true;
		KT_LOG_KF(KT_LOG_IMPORTANCE_LEVEL_PROXY, "DIRTY STATIC command buffer frame %u", frameIndex);

		if (count > 0)
		{
			staticProxies_[frameIndex].Add(proxy);
			--count;
		}
		else if (count < 0)
		{
			staticProxies_[frameIndex].Remove(proxy);
			++count;
		}
	}

	std::erase_if(stagingStaticProxies_,
		[](const std::pair<KtRenderable3DProxy*, int32_t>& pair)
		{
			return pair.second == 0;
		}
	);
}

void KtSceneRenderer::UpdateDynamicProxies(const uint32_t frameIndex)
{
	if (stagingDynamicProxies_.empty())
	{
		return;
	}

	for (auto& [proxy, count] : stagingDynamicProxies_)
	{
		if (count == 0)
		{
			continue;
		}

		isDynamicCommandBufferDirty_[frameIndex] = true;
		KT_LOG_KF(KT_LOG_IMPORTANCE_LEVEL_PROXY, "DIRTY DYNAMIC command buffer frame %u", frameIndex);

		if (count > 0)
		{
			dynamicProxies_[frameIndex].Add(proxy);
			--count;
		}
		else if (count < 0)
		{
			dynamicProxies_[frameIndex].Remove(proxy);
			++count;
		}
	}

	std::erase_if(stagingDynamicProxies_,
		[](const std::pair<KtRenderable3DProxy*, int32_t>& pair)
		{
			return pair.second == 0;
		}
	);
}

void KtSceneRenderer::SortProxies(ProxiesPool& proxies) 
{
	std::sort(proxies.begin(), proxies.end(),
		[](const KtRenderable3DProxy* a, const KtRenderable3DProxy* b)
		{
			if (a->shader != b->shader)
			{
				return a->shader < b->shader;
			}
			if (a->renderable != b->renderable)
			{
				return a->renderable < b->renderable;
			}
			return a->viewport < b->viewport;
		}
	);
}

void KtSceneRenderer::CmdDraw(VkCommandBuffer commandBuffer, const uint32_t frameIndex)
{
	instanceIndices_[frameIndex].clear();
	// draw calls are currently messed up by command buffers record
	// that only records once per frame in flight at every change
	stats_[frameIndex] = {};

	if (isStaticCommandBufferDirty_[frameIndex] || 
		isDynamicCommandBufferDirty_[frameIndex] || 
		GetIsDynamicProxiesDirty(frameIndex))
	{
		const KtCuller3D culler{};
		auto culledStaticProxies = culler.ComputeCulling(staticProxies_[frameIndex], KT_CULLER_3D_FIELD_ALL);
		auto culledDynamicProxies = culler.ComputeCulling(dynamicProxies_[frameIndex], KT_CULLER_3D_FIELD_ALL);
		SortProxies(culledStaticProxies);
		SortProxies(culledDynamicProxies);

		ProxiesPool sortedGlobalProxies = culledStaticProxies;
		sortedGlobalProxies.Merge(culledDynamicProxies);
		
		UpdateDescriptorSetObjectBuffers(sortedGlobalProxies, frameIndex);
		MarkDynamicProxiesNotDirty(frameIndex);
		KT_LOG_KF(KT_LOG_IMPORTANCE_LEVEL_PROXY, "update DESCRIPTOR sets frame %u", frameIndex);
	}

	if (isUniformBufferDirty_[frameIndex])
	{
		isUniformBufferDirty_[frameIndex] = false;
		KT_LOG_KF(KT_LOG_IMPORTANCE_LEVEL_LOW, "update uniform");
		UpdateDescriptorSetUniformBuffers(staticProxies_[frameIndex], frameIndex);
		UpdateDescriptorSetUniformBuffers(dynamicProxies_[frameIndex], frameIndex);
	}

	if (isStaticCommandBufferDirty_[frameIndex])
	{
		isStaticCommandBufferDirty_[frameIndex] = false;
		RecordStaticCommandBuffer(frameIndex);
		KT_LOG_KF(KT_LOG_IMPORTANCE_LEVEL_PROXY, "update STATIC command buffer frame %u", frameIndex);
	}
	if (isDynamicCommandBufferDirty_[frameIndex])
	{
		isDynamicCommandBufferDirty_[frameIndex] = false;
		RecordDynamicCommandBuffer(frameIndex);
		KT_LOG_KF(KT_LOG_IMPORTANCE_LEVEL_PROXY, "update DYNAMIC command buffer frame %u", frameIndex);
	}

	CmdExecuteCommandBuffers(commandBuffer, frameIndex);
}

void KtSceneRenderer::UpdateDescriptorSetObjectBuffers(const ProxiesPool& proxies, const uint32_t frameIndex) const
{
	std::unordered_map<KtShader*, std::vector<KtObjectData3D>> shaderObjectBufferDatas{};
	for (const auto* proxy : proxies)
	{
		shaderObjectBufferDatas[proxy->shader].push_back(proxy->objectData);
	}

	for (const auto& [shader, objectBufferDatas] : shaderObjectBufferDatas)
	{
		if (auto* binding = shader->GetDescriptorSetLayoutBinding("objectBuffer"))
		{
			shader->UpdateDescriptorSetLayoutBindingBufferMemberCount(*binding, objectBufferDatas.size(), frameIndex);
			shader->UpdateDescriptorSetLayoutBindingBuffer(*binding, objectBufferDatas.data(), frameIndex);
		}
	}
}

void KtSceneRenderer::UpdateDescriptorSetUniformBuffers(const ProxiesPool& proxies, const uint32_t frameIndex) const
{
	std::unordered_set<KtShader*> shaders{};
	for (const auto* proxy : proxies)
	{
		shaders.insert(proxy->shader);
	}

	for (auto* shader : shaders)
	{
		if (auto* binding = shader->GetDescriptorSetLayoutBinding("cameraData"))
		{
			shader->UpdateDescriptorSetLayoutBindingBuffer(*binding, &uniformDatas_[frameIndex], frameIndex);
		}
	}
}

void KtSceneRenderer::CmdDrawProxies(VkCommandBuffer commandBuffer, const ProxiesPool& proxies, const uint32_t frameIndex)
{
	if (proxies.Empty())
	{
		return;
	}

	const KtShader* currentShader = nullptr;
	const KtRenderable3D* currentRenderable = nullptr;
	const KtViewport* currentViewport = nullptr;

	for (size_t i = 0; i < proxies.Size();)
	{
		const auto* proxy = proxies[i];
		const KtShader* shader = proxy->shader;
		const KtRenderable3D* renderable = proxy->renderable;
		const KtViewport* viewport = proxy->viewport;

		// Find the extent of the current batch
		size_t instanceCount = 1;
		while (i + instanceCount < proxies.Size())
		{
			const auto* next = proxies[i + instanceCount];
			if (next->shader != shader || next->renderable != renderable || next->viewport != viewport)
			{
				break;
			}
			++instanceCount;
		}

		// Bind new state if necessary
		if (currentShader != shader)
		{
			currentShader = shader;
			currentShader->CmdBind(commandBuffer);
			currentShader->CmdBindDescriptorSets(commandBuffer, frameIndex);
		}

		if (currentRenderable != renderable)
		{
			currentRenderable = renderable;
			currentRenderable->CmdBind(commandBuffer);
		}

		if (currentViewport != viewport)
		{
			currentViewport = viewport;
			currentViewport->CmdUse(commandBuffer);
		}

		// Submit draw
		currentRenderable->CmdDraw(commandBuffer, static_cast<uint32_t>(instanceCount), instanceIndices_[frameIndex][shader]);
		instanceIndices_[frameIndex][shader] += static_cast<uint32_t>(instanceCount);

		i += instanceCount;
	}
}

void KtSceneRenderer::CmdExecuteCommandBuffers(VkCommandBuffer commandBuffer, const uint32_t frameIndex)
{
	const std::array<VkCommandBuffer, 2> commandBuffers =
	{
		staticCommandBuffers_[frameIndex],
		dynamicCommandBuffers_[frameIndex]
	};

	vkCmdExecuteCommands(commandBuffer, static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
}

bool KtSceneRenderer::GetIsDynamicProxiesDirty(const uint32_t frameIndex) const
{
	auto proxies = KtCollection(dynamicProxies_[frameIndex].begin(), dynamicProxies_[frameIndex].end());
	proxies.AddFilter([](const KtRenderable3DProxy* proxy) { return proxy->isDirty; });
	return proxies.GetFirst() != nullptr;
}

void KtSceneRenderer::MarkDynamicProxiesNotDirty(const uint32_t frameIndex)
{
	for (auto* proxy : dynamicProxies_[frameIndex])
	{
		proxy->isDirty = false;
	}
}
