#include "SceneRenderer.h"
#include "Renderer.h"
#include "SceneCuller.h"
#include "SceneRenderable.h"
#include "SceneProxy.h"
#include "Shader.h"
#include <kotono_common/log.h>
#include <kotono_platform/Context.h>
#include <kotono_platform/vk_utils.h>
#include <kotono_platform/WindowViewport.h>
#include <unordered_set>

#define KT_LOG_IMPORTANCE_LEVEL_PROXY ELogImportanceLevel::Medium

void KtSceneRenderer::Init()
{
	CreateStaticCommandBuffers();
	CreateDynamicCommandBuffers();
	isUniformBufferDirty_.fill(true);
	MarkCommandBuffersDirty();
}

void KtSceneRenderer::Update(const uint32_t frameIndex)
{
	UpdateUniformData(frameIndex);
	UpdateStagingStaticProxies(frameIndex);
	UpdateStagingDynamicProxies(frameIndex);
	UpdateStaticProxies(frameIndex);
	UpdateDynamicProxies(frameIndex);
}

void KtSceneRenderer::Cleanup()
{
}

void KtSceneRenderer::MarkCommandBuffersDirty()
{
	isStaticCommandBufferDirty_.fill(true);
	isDynamicCommandBufferDirty_.fill(true);
}

void KtSceneRenderer::SetUniformData(const KtSceneUniformData& uniformData)
{
	stagingUniformData_ = { uniformData, static_cast<uint32_t>(KT_FRAMES_IN_FLIGHT) };
}

void KtSceneRenderer::RegisterProxy(Proxy* proxy, const EMobility mobility)
{
	switch (mobility)
	{
	case EMobility::Dynamic:
		stagingDynamicProxies_[proxy] = static_cast<int32_t>(KT_FRAMES_IN_FLIGHT);
		break;
	case EMobility::Static:
		stagingStaticProxies_[proxy] = static_cast<int32_t>(KT_FRAMES_IN_FLIGHT);
		break;
	}
}

void KtSceneRenderer::UnregisterProxy(Proxy* proxy, const EMobility mobility)
{
	switch (mobility)
	{
	case EMobility::Dynamic:
		stagingDynamicProxies_[proxy] = -static_cast<int32_t>(KT_FRAMES_IN_FLIGHT);
		break;
	case EMobility::Static:
		stagingStaticProxies_[proxy] = -static_cast<int32_t>(KT_FRAMES_IN_FLIGHT);
		break;
	}
}

void KtSceneRenderer::CreateStaticCommandBuffers()
{
	for (size_t i{ 0 }; i < KT_FRAMES_IN_FLIGHT; ++i)
	{
		CreateStaticCommandBuffer(static_cast<uint32_t>(i));
	}
}

void KtSceneRenderer::CreateStaticCommandBuffer(const uint32_t frameIndex)
{
	const VkCommandBufferAllocateInfo allocInfo{
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
		.commandPool = Renderer.GetCommandPool(frameIndex),
		.level = VK_COMMAND_BUFFER_LEVEL_SECONDARY,
		.commandBufferCount = 1,
	};

	VK_CHECK_THROW(
		vkAllocateCommandBuffers(Context.GetDevice(), &allocInfo, &staticCommandBuffers_[frameIndex]),
		"failed to allocate command buffers!"
	);
}

void KtSceneRenderer::CreateDynamicCommandBuffers()
{
	for (size_t i{ 0 }; i < KT_FRAMES_IN_FLIGHT; ++i)
	{
		CreateDynamicCommandBuffer(static_cast<uint32_t>(i));
	}
}

void KtSceneRenderer::CreateDynamicCommandBuffer(const uint32_t frameIndex)
{
	const VkCommandBufferAllocateInfo allocInfo{
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
		.commandPool = Renderer.GetCommandPool(frameIndex),
		.level = VK_COMMAND_BUFFER_LEVEL_SECONDARY,
		.commandBufferCount = 1,
	};
	VK_CHECK_THROW(
		vkAllocateCommandBuffers(Context.GetDevice(), &allocInfo, &dynamicCommandBuffers_[frameIndex]),
		"failed to allocate command buffers!"
	);
}

void KtSceneRenderer::RecordStaticCommandBuffer(const uint32_t frameIndex)
{
	//SortProxies(staticProxies_[frameIndex]);
	VkCommandBuffer commandBuffer{ staticCommandBuffers_[frameIndex] };
	BeginCommandBuffer(commandBuffer, frameIndex);
	CmdDrawProxies(commandBuffer, sortedStaticProxies_[frameIndex], frameIndex);
	EndCommandBuffer(commandBuffer);
}

void KtSceneRenderer::RecordDynamicCommandBuffer(const uint32_t frameIndex)
{
	//SortProxies(dynamicProxies_[frameIndex]);
	VkCommandBuffer commandBuffer{ dynamicCommandBuffers_[frameIndex] };
	BeginCommandBuffer(commandBuffer, frameIndex);
	CmdDrawProxies(commandBuffer, sortedDynamicProxies_[frameIndex], frameIndex);
	EndCommandBuffer(commandBuffer);
}

void KtSceneRenderer::BeginCommandBuffer(VkCommandBuffer commandBuffer, const uint32_t frameIndex)
{
	vkResetCommandBuffer(commandBuffer, 0);

	const VkCommandBufferInheritanceInfo inheritanceInfo{
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO,
		.renderPass = Renderer.RenderPass(),
		.subpass = 0,
		.framebuffer = Renderer.GetFramebuffer(frameIndex),
	};
	const VkCommandBufferBeginInfo beginInfo{
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
		.flags = VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT,
		.pInheritanceInfo = &inheritanceInfo,
	};
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
	for (Proxy* proxy : staticProxies_[frameIndex])
	{
		if (proxy->IsDirty())
		{
			isStaticCommandBufferDirty_[frameIndex] = true;
			proxy->ApplyPendingUpdates(frameIndex);
		}
	}
}

void KtSceneRenderer::UpdateDynamicProxies(const uint32_t frameIndex)
{
	for (Proxy* proxy : dynamicProxies_[frameIndex])
	{
		if (proxy->IsDirty())
		{
			isDynamicCommandBufferDirty_[frameIndex] = true;
			proxy->ApplyPendingUpdates(frameIndex);
		}
	}
}

void KtSceneRenderer::UpdateStagingStaticProxies(const uint32_t frameIndex)
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
		KT_LOG(KT_LOG_IMPORTANCE_LEVEL_PROXY, "Graphics.KtSceneRenderer::UpdateStagingStaticProxies()", "dirty static command buffer frame %u", frameIndex);

		if (count > 0)
		{
			staticProxies_[frameIndex].Add(proxy);
			proxy->index_[frameIndex] = staticProxies_[frameIndex].LastIndex();
			--count;
		}
		else if (count < 0)
		{
			const size_t index{ proxy->index_[frameIndex] };
			if (staticProxies_[frameIndex].RemoveAt(index) == KtPoolRemoveResult::ItemSwappedAndRemoved)
			{
				staticProxies_[frameIndex][index]->index_[frameIndex] = index;
			}
			++count;
		}
	}

	std::erase_if(stagingStaticProxies_,
		[](const std::pair<const Proxy*, int32_t>& pair)
		{
			return pair.second == 0;
		}
	);
}

void KtSceneRenderer::UpdateStagingDynamicProxies(const uint32_t frameIndex)
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
		KT_LOG(KT_LOG_IMPORTANCE_LEVEL_PROXY, "Graphics.KtSceneRenderer::UpdateStagingDynamicProxies()", "dirty dynamic command buffer frame %u", frameIndex);

		if (count > 0)
		{
			dynamicProxies_[frameIndex].Add(proxy);
			proxy->index_[frameIndex] = dynamicProxies_[frameIndex].LastIndex();
			--count;
		}
		else if (count < 0)
		{
			const size_t index{ proxy->index_[frameIndex] };
			if (dynamicProxies_[frameIndex].RemoveAt(index) == KtPoolRemoveResult::ItemSwappedAndRemoved)
			{
				dynamicProxies_[frameIndex][index]->index_[frameIndex] = index;
			}
			++count;
		}
	}

	std::erase_if(stagingDynamicProxies_,
		[](const std::pair<const Proxy*, int32_t>& pair)
		{
			return pair.second == 0;
		}
	);
}

void KtSceneRenderer::SortProxies(ProxiesPool& proxies, const uint32_t frameIndex)
{
	std::sort(proxies.begin(), proxies.end(),
		[frameIndex](const Proxy* a, const Proxy* b)
		{
			const auto& aFrameData{ a->frameDatas_[frameIndex] };
			const auto& bFrameData{ b->frameDatas_[frameIndex] };
			if (aFrameData.shader != bFrameData.shader)
			{
				return aFrameData.shader < bFrameData.shader;
			}
			return aFrameData.renderable < bFrameData.renderable;
		}
	);
}

void KtSceneRenderer::CmdDraw(VkCommandBuffer commandBuffer, const uint32_t frameIndex)
{
	instanceIndices_[frameIndex].clear();
	// draw calls are currently messed up by command buffers record
	// that only records once per frame in flight at every change
	stats_[frameIndex] = {};

	if (isStaticCommandBufferDirty_[frameIndex] || isDynamicCommandBufferDirty_[frameIndex])
	{
		//const KtSceneCuller culler(KT_SCENE_CULLER_FIELD_ALL);
		//ProxiesPool culledStaticProxies{ culler.ComputeCulling(staticProxies_[frameIndex]) };
		//ProxiesPool culledDynamicProxies{ culler.ComputeCulling(dynamicProxies_[frameIndex]) };
		//SortProxies(culledStaticProxies);
		//SortProxies(culledDynamicProxies);
		//
		//ProxiesPool sortedGlobalProxies{ culledStaticProxies };
		//sortedGlobalProxies.Merge(culledDynamicProxies);

		sortedStaticProxies_[frameIndex] = staticProxies_[frameIndex];
		sortedDynamicProxies_[frameIndex] = dynamicProxies_[frameIndex];

		SortProxies(sortedStaticProxies_[frameIndex], frameIndex);
		SortProxies(sortedDynamicProxies_[frameIndex], frameIndex);

		ProxiesPool sortedGlobalProxies{ sortedStaticProxies_[frameIndex] };
		sortedGlobalProxies.Append(sortedDynamicProxies_[frameIndex]);

		UpdateDescriptorSetObjectBuffers(sortedGlobalProxies, frameIndex);
		KT_LOG(KT_LOG_IMPORTANCE_LEVEL_PROXY, "Graphics.KtSceneRenderer::CmdDraw()", "update descriptor sets frame %u", frameIndex);
	}

	if (isUniformBufferDirty_[frameIndex])
	{
		isUniformBufferDirty_[frameIndex] = false;
		KT_LOG(ELogImportanceLevel::Low, "Graphics.KtSceneRenderer::CmdDraw()", "update uniform");
		UpdateDescriptorSetUniformBuffers(staticProxies_[frameIndex], frameIndex);
		UpdateDescriptorSetUniformBuffers(dynamicProxies_[frameIndex], frameIndex);
	}

	if (isStaticCommandBufferDirty_[frameIndex])
	{
		isStaticCommandBufferDirty_[frameIndex] = false;
		RecordStaticCommandBuffer(frameIndex);
		KT_LOG(KT_LOG_IMPORTANCE_LEVEL_PROXY, "Graphics.KtSceneRenderer::CmdDraw()", "update static command buffer frame %u", frameIndex);
	}
	if (isDynamicCommandBufferDirty_[frameIndex])
	{
		isDynamicCommandBufferDirty_[frameIndex] = false;
		RecordDynamicCommandBuffer(frameIndex);
		KT_LOG(KT_LOG_IMPORTANCE_LEVEL_PROXY, "Graphics.KtSceneRenderer::CmdDraw()", "update dynamic command buffer frame %u", frameIndex);
	}

	CmdExecuteCommandBuffers(commandBuffer, frameIndex);
}

USceneProxy* KtSceneRenderer::CreateProxy() const
{
	return new USceneProxy{};
}

void KtSceneRenderer::UpdateDescriptorSetObjectBuffers(const ProxiesPool& proxies, const uint32_t frameIndex) const
{
	std::unordered_map<KtShader*, std::vector<KtSceneObjectData>> shaderObjectBufferDatas{};
	for (const Proxy* proxy : proxies)
	{
		const auto& frameData{ proxy->frameDatas_[frameIndex] };
		shaderObjectBufferDatas[frameData.shader].push_back(frameData.objectData);
	}

	for (const auto& [shader, objectBufferDatas] : shaderObjectBufferDatas)
	{
		if (auto* binding{ shader->GetDescriptorSetLayoutBinding("objectBuffer") })
		{
			shader->UpdateDescriptorSetLayoutBindingBufferMemberCount(*binding, objectBufferDatas.size(), frameIndex);
			shader->UpdateDescriptorSetLayoutBindingBuffer(*binding, objectBufferDatas.data(), frameIndex);
		}
	}
}

void KtSceneRenderer::UpdateDescriptorSetUniformBuffers(const ProxiesPool& proxies, const uint32_t frameIndex) const
{
	std::unordered_set<KtShader*> shaders{};
	for (const Proxy* proxy : proxies)
	{
		const auto& frameData{ proxy->frameDatas_[frameIndex] };
		shaders.insert(frameData.shader);
	}

	for (auto* shader : shaders)
	{
		if (auto* binding{ shader->GetDescriptorSetLayoutBinding("cameraData") })
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

	WindowViewport.CmdUse(commandBuffer);

	const KtShader* currentShader{ nullptr };
	const KtSceneRenderable* currentRenderable{ nullptr };

	for (size_t i{ 0 }; i < proxies.size();)
	{
		const auto& proxyFrameData{ proxies[i]->frameDatas_[frameIndex] };
		const KtShader* shader{ proxyFrameData.shader };
		const KtSceneRenderable* renderable{ proxyFrameData.renderable };
		const KtScissor scissor{ proxyFrameData.scissor };

		// Find the extent of the current batch
		size_t instanceCount{ 1 };
		for (; i + instanceCount < proxies.size(); ++instanceCount)
		{
			const auto& nextFrameData{ proxies[i + instanceCount]->frameDatas_[frameIndex] };
			if (nextFrameData.shader != shader || 
				nextFrameData.renderable != renderable || 
				nextFrameData.scissor.extent != scissor.extent || 
				nextFrameData.scissor.offset != scissor.offset)
			{
				break;
			}
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

		const auto offset{ scissor.offset };
		const auto extent{ scissor.extent };
		const VkRect2D vkScissor{
			.offset = { offset.x, offset.y },
			.extent = { extent.x, extent.y },
		};
		vkCmdSetScissor(commandBuffer, 0, 1, &vkScissor);

		// Submit draw
		currentRenderable->CmdDraw(commandBuffer, static_cast<uint32_t>(instanceCount), instanceIndices_[frameIndex][shader]);
		instanceIndices_[frameIndex][shader] += static_cast<uint32_t>(instanceCount);

		i += instanceCount;
	}
}

void KtSceneRenderer::CmdExecuteCommandBuffers(VkCommandBuffer commandBuffer, const uint32_t frameIndex)
{
	const std::array<VkCommandBuffer, 2> commandBuffers 
	{
		staticCommandBuffers_[frameIndex],
		dynamicCommandBuffers_[frameIndex]
	};

	vkCmdExecuteCommands(commandBuffer, static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
}
