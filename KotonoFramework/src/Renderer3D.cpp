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

#define KT_LOG_IMPORTANCE_LEVEL_PROXY KT_LOG_IMPORTANCE_LEVEL_NONE

void KtRenderer3D::Init()
{
	CreateStaticCommandBuffers();
	CreateDynamicCommandBuffers();
	isStaticCommandBufferDirty_.fill(true);
	isDynamicCommandBufferDirty_.fill(true);
}

void KtRenderer3D::Update(const uint32_t frameIndex)
{
	UpdateStaticProxies(frameIndex);
	UpdateDynamicProxies(frameIndex);
}

void KtRenderer3D::Cleanup()
{
}

void KtRenderer3D::SetUniformData(const KtUniformData3D& uniformData)
{
	uniformDatas_[Framework.GetRenderer().GetGameThreadFrame()] = uniformData;
}

void KtRenderer3D::RegisterStatic(KtRenderable3DProxy* proxy)
{
	stagingStaticProxies_[proxy] = static_cast<int32_t>(KT_FRAMES_IN_FLIGHT);
}

void KtRenderer3D::RegisterDynamic(KtRenderable3DProxy* proxy)
{
	stagingDynamicProxies_[proxy] = static_cast<int32_t>(KT_FRAMES_IN_FLIGHT);
}

void KtRenderer3D::RemoveStatic(KtRenderable3DProxy* proxy)
{
	stagingStaticProxies_[proxy] = -static_cast<int32_t>(KT_FRAMES_IN_FLIGHT);
}

void KtRenderer3D::RemoveDynamic(KtRenderable3DProxy* proxy)
{
	stagingDynamicProxies_[proxy] = -static_cast<int32_t>(KT_FRAMES_IN_FLIGHT);
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

void KtRenderer3D::RecordStaticCommandBuffer(const uint32_t frameIndex)
{
	sortedStaticProxies_[frameIndex] = GetSortedProxies(staticProxies_[frameIndex]);
	VkCommandBuffer commandBuffer = staticCommandBuffers_[frameIndex];
	BeginCommandBuffer(commandBuffer, frameIndex);
	CmdDrawProxies(commandBuffer, sortedStaticProxies_[frameIndex], frameIndex);
	EndCommandBuffer(commandBuffer);
}

void KtRenderer3D::RecordDynamicCommandBuffer(const uint32_t frameIndex)
{
	sortedDynamicProxies_[frameIndex] = GetSortedProxies(dynamicProxies_[frameIndex]);
	VkCommandBuffer commandBuffer = dynamicCommandBuffers_[frameIndex];
	BeginCommandBuffer(commandBuffer, frameIndex);
	CmdDrawProxies(commandBuffer, sortedDynamicProxies_[frameIndex], frameIndex);
	EndCommandBuffer(commandBuffer);
}

void KtRenderer3D::BeginCommandBuffer(VkCommandBuffer commandBuffer, const uint32_t frameIndex)
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

void KtRenderer3D::EndCommandBuffer(VkCommandBuffer commandBuffer)
{
	VK_CHECK_THROW(
		vkEndCommandBuffer(commandBuffer),
		"failed to record command buffer!"
	);
}

void KtRenderer3D::UpdateStaticProxies(const uint32_t frameIndex)
{
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
			staticProxies_[frameIndex].insert(proxy);
			--count;
		}
		else if (count < 0)
		{
			staticProxies_[frameIndex].erase(proxy);
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

void KtRenderer3D::UpdateDynamicProxies(const uint32_t frameIndex)
{
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
			dynamicProxies_[frameIndex].insert(proxy);
			--count;
		}
		else if (count < 0)
		{
			dynamicProxies_[frameIndex].erase(proxy);
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

const KtRenderer3D::ProxiesVector KtRenderer3D::GetSortedProxies(const ProxiesUnorderedSet& proxies) const
{
	ProxiesVector sortedProxies(proxies.begin(), proxies.end());

	std::sort(sortedProxies.begin(), sortedProxies.end(),
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

	return sortedProxies;
}

void KtRenderer3D::CmdDraw(VkCommandBuffer commandBuffer, const uint32_t frameIndex)
{
	instanceIndices_[frameIndex] = {};
	// draw calls are currently messed up by command buffers record
	// that only records once per frame in flight at every change
	stats_[frameIndex] = {};

	if (isStaticCommandBufferDirty_[frameIndex] || 
		isDynamicCommandBufferDirty_[frameIndex] || 
		GetIsDynamicProxiesDirty(frameIndex))
	{
		const KtCuller3D culler{};
		const auto culledStaticProxies = culler.ComputeCulling(staticProxies_[frameIndex], KT_CULLER_3D_FIELD_ALL);
		const auto culledDynamicProxies = culler.ComputeCulling(dynamicProxies_[frameIndex], KT_CULLER_3D_FIELD_ALL);

		ProxiesVector sortedGlobalProxies = GetSortedProxies(culledStaticProxies);
		const ProxiesVector sortedDynamicProxies = GetSortedProxies(culledDynamicProxies);
		sortedGlobalProxies.insert(sortedGlobalProxies.end(), sortedDynamicProxies.begin(), sortedDynamicProxies.end());
		
		UpdateDescriptorSets(sortedGlobalProxies, frameIndex);
		KT_LOG_KF(KT_LOG_IMPORTANCE_LEVEL_PROXY, "update DESCRIPTOR sets frame %u", frameIndex);
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

void KtRenderer3D::UpdateDescriptorSets(const ProxiesVector& proxies, const uint32_t frameIndex) const
{
	std::unordered_map<KtShader*, std::vector<KtObjectData3D>> shaderObjectBufferDatas;
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
		if (auto* binding = shader->GetDescriptorSetLayoutBinding("cameraData"))
		{
			shader->UpdateDescriptorSetLayoutBindingBuffer(*binding, &uniformDatas_[frameIndex], frameIndex);
		}
	}
}

void KtRenderer3D::CmdDrawProxies(VkCommandBuffer commandBuffer, const ProxiesVector& proxies, const uint32_t frameIndex)
{
	if (proxies.empty())
	{
		return;
	}

	const KtShader* currentShader = nullptr;
	const KtRenderable3D* currentRenderable = nullptr;
	const KtViewport* currentViewport = nullptr;
	
	for (size_t i = 0; i < proxies.size();)
	{
		const auto* proxy = proxies[i];
		const KtShader* shader = proxy->shader;
		const KtRenderable3D* renderable = proxy->renderable;
		const KtViewport* viewport = proxy->viewport;

		// Find the extent of the current batch
		size_t instanceCount = 1;
		while (i + instanceCount < proxies.size())
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

void KtRenderer3D::CmdExecuteCommandBuffers(VkCommandBuffer commandBuffer, const uint32_t frameIndex)
{
	const std::array<VkCommandBuffer, 2> commandBuffers =
	{
		staticCommandBuffers_[frameIndex],
		dynamicCommandBuffers_[frameIndex]
	};

	vkCmdExecuteCommands(commandBuffer, static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
}

const bool KtRenderer3D::GetIsDynamicProxiesDirty(const uint32_t frameIndex) const
{
	auto proxies = KtCollection(dynamicProxies_[frameIndex].begin(), dynamicProxies_[frameIndex].end());
	proxies.AddFilter([](const KtRenderable3DProxy* proxy) { return proxy->isDirty; });
	return proxies.GetFirst() != nullptr;
}
