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
	CreateCommandBuffers();
	MarkUniformBuffersDirty();
	MarkObjectBuffersDirty();
}

void KtSceneRenderer::Update(const uint32_t frameIndex)
{
	UpdateUniformData(frameIndex);
	UpdateStagingProxies(stagingStaticProxies_, frameDatas_[frameIndex].staticBuffer, frameIndex);
	UpdateStagingProxies(stagingDynamicProxies_, frameDatas_[frameIndex].dynamicBuffer, frameIndex);
	UpdateProxies(frameDatas_[frameIndex].staticBuffer, frameIndex);
	UpdateProxies(frameDatas_[frameIndex].dynamicBuffer, frameIndex);
	DeleteProxies();
}

void KtSceneRenderer::Cleanup()
{
}

void KtSceneRenderer::MarkUniformBuffersDirty()
{
	for (auto& frameData : frameDatas_)
	{
		frameData.uniformBuffer.isDirty = true;
	}
}

void KtSceneRenderer::MarkObjectBuffersDirty()
{
	for (auto& frameData : frameDatas_)
	{
		frameData.staticBuffer.isDirty = true;
		frameData.dynamicBuffer.isDirty = true;
	}
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

void KtSceneRenderer::CmdDraw(VkCommandBuffer commandBuffer, const uint32_t frameIndex)
{
	auto& frameData{ frameDatas_[frameIndex] };

	KT_LOG(ELogImportanceLevel::Low, "Graphics.KtSceneRenderer::CmdDraw()",
		"%llu static proxies", frameData.staticBuffer.proxies.size());
	KT_LOG(ELogImportanceLevel::Low, "Graphics.KtSceneRenderer::CmdDraw()",
		"%llu dynamic proxies", frameData.dynamicBuffer.proxies.size());

	if (frameData.staticBuffer.isDirty || frameData.dynamicBuffer.isDirty)
	{
		frameData.instanceIndices.clear();
		frameData.stats = {};

		frameData.staticBuffer.sortedProxies = frameData.staticBuffer.proxies;
		frameData.dynamicBuffer.sortedProxies = frameData.dynamicBuffer.proxies;

		SortProxies(frameData.staticBuffer.sortedProxies, frameIndex);
		SortProxies(frameData.dynamicBuffer.sortedProxies, frameIndex);

		ProxiesPool sortedGlobalProxies{ frameData.staticBuffer.sortedProxies };
		sortedGlobalProxies.Append(frameData.dynamicBuffer.sortedProxies);

		UpdateDescriptorSetObjectBuffers(sortedGlobalProxies, frameIndex);
		KT_LOG(KT_LOG_IMPORTANCE_LEVEL_PROXY, "Graphics.KtSceneRenderer::CmdDraw()", "update descriptor sets frame %u", frameIndex);

		frameData.staticBuffer.drawBatches = GetDrawBatches(frameData.staticBuffer.sortedProxies, frameIndex);
		frameData.dynamicBuffer.drawBatches = GetDrawBatches(frameData.dynamicBuffer.sortedProxies, frameIndex);
		UpdateIndirectBuffers(frameData.staticBuffer.drawBatches, frameIndex);
		UpdateIndirectBuffers(frameData.dynamicBuffer.drawBatches, frameIndex);
	}

	if (frameData.uniformBuffer.isDirty)
	{
		frameData.uniformBuffer.isDirty = false;
		KT_LOG(ELogImportanceLevel::Low, "Graphics.KtSceneRenderer::CmdDraw()", "update uniform");
		UpdateDescriptorSetUniformBuffers(frameData.staticBuffer.proxies, frameIndex);
		UpdateDescriptorSetUniformBuffers(frameData.dynamicBuffer.proxies, frameIndex);
	}

	if (frameData.staticBuffer.isDirty)
	{
		frameData.staticBuffer.isDirty = false;
		RecordCommandBuffer(frameData.staticBuffer, frameIndex);
		KT_LOG(KT_LOG_IMPORTANCE_LEVEL_PROXY, "Graphics.KtSceneRenderer::CmdDraw()", "update static command buffer frame %u", frameIndex);
	}
	if (frameData.dynamicBuffer.isDirty)
	{
		frameData.dynamicBuffer.isDirty = false;
		RecordCommandBuffer(frameData.dynamicBuffer, frameIndex);
		KT_LOG(KT_LOG_IMPORTANCE_LEVEL_PROXY, "Graphics.KtSceneRenderer::CmdDraw()", "update dynamic command buffer frame %u", frameIndex);
	}

	CmdExecuteCommandBuffers(commandBuffer, frameIndex);
}

USceneProxy* KtSceneRenderer::CreateProxy() const
{
	return new USceneProxy{};
}

void KtSceneRenderer::DeleteProxy(Proxy* proxy)
{
	deleteProxies_[proxy] = static_cast<uint32_t>(KT_FRAMES_IN_FLIGHT);
}

void KtSceneRenderer::CreateCommandBuffers()
{
	for (size_t i{ 0 }; i < KT_FRAMES_IN_FLIGHT; ++i)
	{
		CreateCommandBuffer(frameDatas_[i].staticBuffer, static_cast<uint32_t>(i));
		CreateCommandBuffer(frameDatas_[i].dynamicBuffer, static_cast<uint32_t>(i));
	}
}

void KtSceneRenderer::CreateCommandBuffer(FrameData::ObjectBufferData& objectBuffer, const uint32_t frameIndex)
{
	const VkCommandBufferAllocateInfo allocInfo{
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
		.commandPool = Renderer.GetCommandPool(frameIndex),
		.level = VK_COMMAND_BUFFER_LEVEL_SECONDARY,
		.commandBufferCount = 1,
	};

	VK_CHECK_THROW(
		vkAllocateCommandBuffers(Context.GetDevice(), &allocInfo, &objectBuffer.commandBuffer),
		"failed to allocate command buffers!"
	);
}

void KtSceneRenderer::RecordCommandBuffer(const FrameData::ObjectBufferData& objectBuffer, const uint32_t frameIndex)
{
	VkCommandBuffer commandBuffer{ objectBuffer.commandBuffer };
	BeginCommandBuffer(commandBuffer, frameIndex);
	CmdDrawProxies(commandBuffer, objectBuffer.drawBatches, frameIndex);
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

	frameDatas_[frameIndex].uniformBuffer.isDirty = true;
	frameDatas_[frameIndex].uniformBuffer.uniformData = stagingUniformData_.first;
	--stagingUniformData_.second;
}

void KtSceneRenderer::UpdateProxies(FrameData::ObjectBufferData& objectBuffer, const uint32_t frameIndex)
{
	for (Proxy* proxy : objectBuffer.proxies)
	{
		if (proxy->IsDirty())
		{
			objectBuffer.isDirty = true;
			proxy->ApplyPendingUpdates(frameIndex);
		}
	}
}

void KtSceneRenderer::UpdateStagingProxies(StagingProxiesMap& stagingProxies, FrameData::ObjectBufferData& objectBuffer, const uint32_t frameIndex)
{
	if (stagingProxies.empty())
	{
		return;
	}

	for (auto& [proxy, count] : stagingProxies)
	{
		if (count == 0)
		{
			continue;
		}

		objectBuffer.isDirty = true;
		KT_LOG(KT_LOG_IMPORTANCE_LEVEL_PROXY, "Graphics.KtSceneRenderer::UpdateStagingProxies()", "dirty command buffer frame %u", frameIndex);

		if (count > 0)
		{
			if (!proxy->frameDatas_[frameIndex].poolData.isRegistered)
			{
				proxy->frameDatas_[frameIndex].poolData.isRegistered = true;
				objectBuffer.proxies.Add(proxy);
				proxy->frameDatas_[frameIndex].poolData.index = objectBuffer.proxies.LastIndex();
			}

			--count;
		}
		else if (count < 0)
		{
			if (proxy->frameDatas_[frameIndex].poolData.isRegistered)
			{
				proxy->frameDatas_[frameIndex].poolData.isRegistered = false;
				const size_t index{ proxy->frameDatas_[frameIndex].poolData.index };
				if (objectBuffer.proxies.RemoveAt(index) == KtPoolRemoveResult::ItemSwappedAndRemoved)
				{
					objectBuffer.proxies[index]->frameDatas_[frameIndex].poolData.index = index;
				}
			}

			++count;
		}
	}

	std::erase_if(stagingProxies,
		[](const std::pair<const Proxy*, int32_t>& pair)
		{
			return pair.second == 0;
		}
	);
}

void KtSceneRenderer::UpdateDescriptorSetObjectBuffers(const ProxiesPool& proxies, const uint32_t frameIndex) const
{
	std::unordered_map<KtShader*, std::vector<KtSceneObjectData>> shaderObjectBufferDatas{};
	for (const Proxy* proxy : proxies)
	{
		const auto& frameData{ proxy->frameDatas_[frameIndex] };
		shaderObjectBufferDatas[frameData.data.shader].push_back(frameData.data.objectData);
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
		shaders.insert(frameData.data.shader);
	}

	for (auto* shader : shaders)
	{
		if (auto* binding{ shader->GetDescriptorSetLayoutBinding("cameraData") })
		{
			shader->UpdateDescriptorSetLayoutBindingBuffer(*binding, &frameDatas_[frameIndex].uniformBuffer.uniformData, frameIndex);
		}
	}
}

void KtSceneRenderer::DeleteProxies()
{
	for (auto& [proxy, count] : deleteProxies_)
	{
		--count;
		if (count == 0)
		{
			delete proxy;
		}
	}

	std::erase_if(deleteProxies_,
		[](const std::pair<const Proxy*, uint32_t>& pair)
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
			if (aFrameData.data.shader != bFrameData.data.shader)
			{
				return aFrameData.data.shader < bFrameData.data.shader;
			}
			return aFrameData.data.renderable < bFrameData.data.renderable;
		}
	);
}

void KtSceneRenderer::CmdDrawProxies(VkCommandBuffer commandBuffer, const std::vector<DrawBatch>& drawBatches, const uint32_t frameIndex)
{
	if (drawBatches.empty())
	{
		return;
	}

	WindowViewport.CmdUse(commandBuffer);

	const KtShader* currentShader{ nullptr };
	const KtSceneRenderable* currentRenderable{ nullptr };
	const KtScissor currentScissor{};

	for (auto& drawBatch : drawBatches)
	{
		// Bind new state if necessary
		if (currentShader != drawBatch.shader)
		{
			currentShader = drawBatch.shader;
			currentShader->CmdBind(commandBuffer);
			currentShader->CmdBindDescriptorSets(commandBuffer, frameIndex);
		}

		if (currentRenderable != drawBatch.renderable)
		{
			currentRenderable = drawBatch.renderable;
			currentRenderable->CmdBind(commandBuffer);
		}

		if (currentScissor != drawBatch.scissor)
		{
			const auto offset{ drawBatch.scissor.offset };
			const auto extent{ drawBatch.scissor.extent };
			const VkRect2D vkScissor{
				.offset = { offset.x, offset.y },
				.extent = { extent.x, extent.y },
			};
			vkCmdSetScissor(commandBuffer, 0, 1, &vkScissor);
		}

		currentRenderable->CmdDraw(commandBuffer, frameIndex);
	}
}

void KtSceneRenderer::CmdExecuteCommandBuffers(VkCommandBuffer commandBuffer, const uint32_t frameIndex)
{
	const std::array<VkCommandBuffer, 2> commandBuffers 
	{
		frameDatas_[frameIndex].staticBuffer.commandBuffer,
		frameDatas_[frameIndex].dynamicBuffer.commandBuffer
	};

	vkCmdExecuteCommands(commandBuffer, static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
}

std::vector<KtSceneRenderer::DrawBatch> KtSceneRenderer::GetDrawBatches(const ProxiesPool& proxies, const uint32_t frameIndex)
{
	std::vector<DrawBatch> result{};

	for (size_t i{ 0 }; i < proxies.size();)
	{
		const auto& frameData{ proxies[i]->frameDatas_[frameIndex] };
		KtShader* shader{ frameData.data.shader };
		KtSceneRenderable* renderable{ frameData.data.renderable };
		KtScissor scissor{ frameData.data.scissor };

		// Find the extent of the current batch
		size_t instanceCount{ 1 };
		for (; i + instanceCount < proxies.size(); ++instanceCount)
		{
			const auto& nextFrameData{ proxies[i + instanceCount]->frameDatas_[frameIndex] };
			if (nextFrameData.data.shader != shader ||
				nextFrameData.data.renderable != renderable ||
				nextFrameData.data.scissor != scissor)
			{
				break;
			}
		}

		const DrawBatch batch{
			.shader = shader,
			.renderable = renderable,
			.scissor = scissor,
			.firstInstance = frameDatas_[frameIndex].instanceIndices[shader],
			.instanceCount = static_cast<uint32_t>(instanceCount),
		};
		result.push_back(batch);

		frameDatas_[frameIndex].instanceIndices[shader] += static_cast<uint32_t>(instanceCount);
		i += instanceCount;
	}

	return result;
}

void KtSceneRenderer::UpdateIndirectBuffers(const std::vector<DrawBatch>& drawBatches, const uint32_t frameIndex)
{
	for (auto& drawBatch : drawBatches)
	{
		drawBatch.renderable->UpdateIndirectBuffer(drawBatch.firstInstance, drawBatch.instanceCount, frameIndex);
	}
}
