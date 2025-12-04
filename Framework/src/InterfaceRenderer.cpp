#include "InterfaceRenderer.h"
#include "Framework.h"
#include "log.h"
#include "Context.h"
#include "Vertex2D.h"
#include "InterfaceCuller.h"
#include "Renderer.h"
#include "ImageTexture.h"
#include "Shader.h"
#include "WindowViewport.h"
#include "vk_utils.h"
#include "InterfaceRenderableProxy.h"
#include "Collection.h"

static constexpr std::array<KtVertex2D, 4> Vertices
{//                   Position,              KtColor,      TexCoords
	KtVertex2D { {-0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f} }, // Bottom-left
	KtVertex2D { { 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f} }, // Bottom-right
	KtVertex2D { { 0.5f,  0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f} }, // Top-right
	KtVertex2D { {-0.5f,  0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f} }  // Top-left
};

static constexpr std::array<uint32_t, 6> Indices{ 0, 1, 2, 2, 3, 0 };

void KtInterfaceRenderer::Init()
{
	CreateVertexBuffer();
	CreateIndexBuffer();
	CreateCommandBuffers();
	isCommandBufferDirty_.fill(true);
}

void KtInterfaceRenderer::Update(const uint32_t frameIndex)
{
	if (stagingProxies_.empty())
	{
		return;
	}

	for (auto& [proxy, states] : stagingProxies_)
	{
		auto& state{ states[frameIndex] };

		if (state == StagingProxyState::None)
		{
			continue;
		}

		isCommandBufferDirty_[frameIndex] = true;

		if (state == StagingProxyState::Add)
		{
			proxies_[frameIndex].Add(proxy);
		}
		else if (state == StagingProxyState::Remove)
		{
			proxies_[frameIndex].Remove(proxy);
		}

		state = StagingProxyState::None;
	}

	std::erase_if(stagingProxies_,
		[](const std::pair<KtInterfaceRenderableProxy*, KtFramesInFlightArray<StagingProxyState>>& pair)
		{
			const auto states{ pair.second };
			return std::all_of(states.begin(), states.end(),
				[](const StagingProxyState state)
				{
					return state == StagingProxyState::None;
				}
			);
		}
	);
	
	for (int64_t i{ deleteProxies_.LastIndex() }; i >= 0; --i)
	{
		auto* proxy = deleteProxies_[i];
		if (!stagingProxies_.contains(proxy))
		{
			delete proxy;
			deleteProxies_.RemoveAt(i);
		}
	}
}

void KtInterfaceRenderer::Cleanup() const
{
	KT_LOG_KF(KT_LOG_IMPORTANCE_LEVEL_HIGH, "cleaning up 2D renderer");
	vmaDestroyBuffer(Framework.Context().GetAllocator(), indexBuffer_.Buffer, indexBuffer_.Allocation);
	vmaDestroyBuffer(Framework.Context().GetAllocator(), vertexBuffer_.Buffer, vertexBuffer_.Allocation);
	KT_LOG_KF(KT_LOG_IMPORTANCE_LEVEL_HIGH, "cleaned up 2D renderer");
}

void KtInterfaceRenderer::CreateVertexBuffer()
{
	const VkDeviceSize bufferSize{ sizeof(KtVertex2D) * Vertices.size() };

	Framework.Context().CreateBuffer(
		bufferSize,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT,
		stagingVertexBuffer_
	);

	memcpy(stagingVertexBuffer_.AllocationInfo.pMappedData, Vertices.data(), static_cast<size_t>(bufferSize));

	Framework.Context().CreateBuffer(
		bufferSize,
		VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT,
		vertexBuffer_
	);

	Framework.Context().CopyBuffer(stagingVertexBuffer_.Buffer, vertexBuffer_.Buffer, bufferSize);
	Framework.Context().GetEventExecuteSingleTimeCommands().AddListener(KtDelegate(this, &KtInterfaceRenderer::DestroyStagingVertexBuffer));
}

void KtInterfaceRenderer::CreateIndexBuffer()
{
	const VkDeviceSize bufferSize{ sizeof(uint32_t) * Indices.size() };

	Framework.Context().CreateBuffer(
		bufferSize,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT,
		stagingIndexBuffer_
	);

	memcpy(stagingIndexBuffer_.AllocationInfo.pMappedData, Indices.data(), static_cast<size_t>(bufferSize));

	Framework.Context().CreateBuffer(
		bufferSize,
		VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT,
		indexBuffer_
	);

	Framework.Context().CopyBuffer(stagingIndexBuffer_.Buffer, indexBuffer_.Buffer, bufferSize);
	Framework.Context().GetEventExecuteSingleTimeCommands().AddListener(KtDelegate(this, &KtInterfaceRenderer::DestroyStagingIndexBuffer));
}

void KtInterfaceRenderer::DestroyStagingVertexBuffer() const
{
	vmaDestroyBuffer(Framework.Context().GetAllocator(), stagingVertexBuffer_.Buffer, stagingVertexBuffer_.Allocation);
}

void KtInterfaceRenderer::DestroyStagingIndexBuffer() const
{
	vmaDestroyBuffer(Framework.Context().GetAllocator(), stagingIndexBuffer_.Buffer, stagingIndexBuffer_.Allocation);
}

void KtInterfaceRenderer::CmdBindVertexBuffer(VkCommandBuffer commandBuffer) const
{
	const std::array<VkBuffer, 1> vertexBuffers{ vertexBuffer_.Buffer };
	const std::array<VkDeviceSize, 1> offsets{ 0 };
	vkCmdBindVertexBuffers(commandBuffer, 0, static_cast<uint32_t>(vertexBuffers.size()), vertexBuffers.data(), offsets.data());
}

void KtInterfaceRenderer::CmdBindIndexBuffer(VkCommandBuffer commandBuffer) const
{
	vkCmdBindIndexBuffer(commandBuffer, indexBuffer_.Buffer, 0, VK_INDEX_TYPE_UINT32);
}

void KtInterfaceRenderer::CreateCommandBuffers()
{
	for (size_t i{ 0 }; i < KT_FRAMES_IN_FLIGHT; ++i)
	{
		CreateCommandBuffer(static_cast<uint32_t>(i));
	}
}

void KtInterfaceRenderer::CreateCommandBuffer(const uint32_t frameIndex)
{
	const VkCommandBufferAllocateInfo allocInfo{
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
		.commandPool = Framework.Renderer().GetCommandPool(frameIndex),
		.level = VK_COMMAND_BUFFER_LEVEL_SECONDARY,
		.commandBufferCount = 1,
	};
 
	VK_CHECK_THROW(
		vkAllocateCommandBuffers(Framework.Context().GetDevice(), &allocInfo, &commandBuffers_[frameIndex]),
		"failed to allocate command buffers!"
	);
}

void KtInterfaceRenderer::RecordCommandBuffer(const ProxiesPool& proxies, const uint32_t frameIndex)
{
	VkCommandBuffer commandBuffer{ commandBuffers_[frameIndex] };
	BeginCommandBuffer(commandBuffer, frameIndex);
	CmdDrawProxies(commandBuffer, proxies, frameIndex);
	EndCommandBuffer(commandBuffer);
}

void KtInterfaceRenderer::BeginCommandBuffer(VkCommandBuffer commandBuffer, const uint32_t frameIndex)
{
	vkResetCommandBuffer(commandBuffer, 0);

	const VkCommandBufferInheritanceInfo inheritanceInfo{
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO,
		.renderPass = Framework.Renderer().GetRenderPass(),
		.subpass = 0,
		.framebuffer = Framework.Renderer().GetFramebuffer(frameIndex),
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

void KtInterfaceRenderer::EndCommandBuffer(VkCommandBuffer commandBuffer)
{
	VK_CHECK_THROW(
		vkEndCommandBuffer(commandBuffer),
		"failed to record command buffer!"
	);
}

void KtInterfaceRenderer::SetUniformData(const KtInterfaceUniformData& uniformData)
{
	uniformDatas_[Framework.Renderer().GetGameThreadFrame()] = uniformData;
}

void KtInterfaceRenderer::Register(KtInterfaceRenderableProxy* proxy)
{
	stagingProxies_[proxy].fill(StagingProxyState::Add);
}

void KtInterfaceRenderer::Unregister(KtInterfaceRenderableProxy* proxy)
{
	stagingProxies_[proxy].fill(StagingProxyState::Remove);
}

void KtInterfaceRenderer::CmdDraw(VkCommandBuffer commandBuffer, const uint32_t frameIndex)
{
	if (isCommandBufferDirty_[frameIndex] || GetIsAnyProxyDirty(frameIndex))
	{
		isCommandBufferDirty_[frameIndex] = false;

		const KtInterfaceCuller culler{};
		ProxiesPool culledData{ culler.ComputeCulling(proxies_[frameIndex]) };
		SortProxies(culledData);

		instanceIndices_[frameIndex].clear();
		UpdateDescriptorSets(culledData, frameIndex);

		RecordCommandBuffer(culledData, frameIndex);
		MarkProxiesNotDirty(frameIndex);
	}

	vkCmdExecuteCommands(commandBuffer, 1, &commandBuffers_[frameIndex]);
}

KtInterfaceRenderableProxy* KtInterfaceRenderer::CreateProxy()
{
	return new KtInterfaceRenderableProxy{};
}

void KtInterfaceRenderer::DeleteProxy(KtInterfaceRenderableProxy* proxy)
{
	deleteProxies_.Add(proxy);
}

void KtInterfaceRenderer::UpdateDescriptorSets(const ProxiesPool& proxies, const uint32_t frameIndex)
{
	struct ShaderData final
	{
		std::vector<KtObjectData2D> objectBufferDatas;
		std::vector<KtInterfaceRenderable*> renderables;
		std::vector<uint32_t> renderableIndices;
	};

	std::unordered_map<KtShader*, ShaderData> shaderDatas{};

	for (const auto* proxy : proxies)
	{
		auto& shaderData{ shaderDatas[proxy->shader] };

		shaderData.objectBufferDatas.push_back(proxy->objectData);

		const auto it{ std::find(shaderData.renderables.begin(), shaderData.renderables.end(), proxy->renderable) };

		size_t index;
		if (it == shaderData.renderables.end())
		{
			index = shaderData.renderables.size();
			shaderData.renderables.push_back(proxy->renderable);
		}
		else
		{
			index = std::distance(shaderData.renderables.begin(), it);
		}

		shaderData.renderableIndices.push_back(static_cast<uint32_t>(index));
	}

	for (const auto& [shader, shaderData] : shaderDatas)
	{
		if (auto* binding{ shader->GetDescriptorSetLayoutBinding("objectBuffer") })
		{
			shader->UpdateDescriptorSetLayoutBindingBufferMemberCount(*binding, shaderData.objectBufferDatas.size(), frameIndex);
			shader->UpdateDescriptorSetLayoutBindingBuffer(*binding, shaderData.objectBufferDatas.data(), frameIndex);
		}

		if (auto* binding{ shader->GetDescriptorSetLayoutBinding("textureIndexBuffer") })
		{
			shader->UpdateDescriptorSetLayoutBindingBufferMemberCount(*binding, shaderData.renderableIndices.size(), frameIndex);
			shader->UpdateDescriptorSetLayoutBindingBuffer(*binding, shaderData.renderableIndices.data(), frameIndex);
		}

		if (auto* binding{ shader->GetDescriptorSetLayoutBinding("textures") })
		{
			std::vector<VkDescriptorImageInfo> imageInfos{};
			imageInfos.reserve(shaderData.renderables.size());
			for (const auto* renderable : shaderData.renderables)
			{
				// static_cast is safe because 'textures' expects only KtImageTexture
				const auto* asImageTexture{ static_cast<const KtImageTexture*>(renderable) };
				imageInfos.push_back(asImageTexture->GetDescriptorImageInfo());
			}
			shader->UpdateDescriptorSetLayoutBindingImageSampler(*binding, imageInfos, frameIndex);
		}
	}
}

void KtInterfaceRenderer::CmdDrawProxies(VkCommandBuffer commandBuffer, const ProxiesPool& proxies, const uint32_t frameIndex)
{
	const KtShader* currentShader{ nullptr };

	WindowViewport.CmdUse(commandBuffer);

	CmdBindVertexBuffer(commandBuffer);
	CmdBindIndexBuffer(commandBuffer);

	for (size_t i{ 0 }; i < proxies.size();)
	{
		const KtInterfaceRenderableProxy* proxy{ proxies[i] };
		const KtShader* shader{ proxy->shader };
		const int32_t layer{ proxy->layer };
		const KtScissor scissor{ proxy->scissor };

		size_t instanceCount{ 1 };
		while (i + instanceCount < proxies.size())
		{
			const auto* next{ proxies[i + instanceCount] };
			if (next->shader != shader || next->scissor.extent != scissor.extent || next->scissor.offset != scissor.offset)
			{
				break;
			}
			++instanceCount;
		}

		if (currentShader != shader)
		{
			currentShader = shader;
			currentShader->CmdBind(commandBuffer);
			currentShader->CmdBindDescriptorSets(commandBuffer, frameIndex);
		}

		const auto offset{ scissor.offset };
		const auto extent{ scissor.extent };
		const VkRect2D vkScissor{
			.offset = { offset.x, offset.y },
			.extent = { extent.x, extent.y },
		};
		vkCmdSetScissor(commandBuffer, 0, 1, &vkScissor);
		
		vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(Indices.size()), static_cast<uint32_t>(instanceCount), 0, 0, instanceIndices_[frameIndex][shader]);
		instanceIndices_[frameIndex][shader] += static_cast<uint32_t>(instanceCount);

		i += instanceCount;
	}
}

void KtInterfaceRenderer::SortProxies(ProxiesPool& proxies)
{
	std::sort(proxies.begin(), proxies.end(),
		[](const KtInterfaceRenderableProxy* a, const KtInterfaceRenderableProxy* b)
		{
			if (a->layer != b->layer)
			{
				return a->layer < b->layer;
			}
			if (a->shader != b->shader)
			{
				return a->shader < b->shader;
			}
			return a->renderable < b->renderable;
		}
	);
}

bool KtInterfaceRenderer::GetIsAnyProxyDirty(const uint32_t frameIndex) const
{
	auto proxies{ KtCollection(proxies_[frameIndex].begin(), proxies_[frameIndex].end()) };
	proxies.AddFilter([](const KtInterfaceRenderableProxy* proxy) { return proxy->isDirty; });
	return proxies.GetFirst() != nullptr;
}

void KtInterfaceRenderer::MarkProxiesNotDirty(const uint32_t frameIndex)
{
	for (auto* proxy : proxies_[frameIndex])
	{
		proxy->isDirty = false;
	}
}
