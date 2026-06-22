#include "InterfaceRenderer.h"
#include "Texture.h"
#include "InterfaceCuller.h"
#include "InterfaceProxy.h"
#include "Renderer.h"
#include "Shader.h"
#include "Vertex2D.h"
#include <kotono_common/log.h>
#include <kotono_platform/Context.h>
#include <kotono_platform/vk_utils.h>
#include <kotono_platform/WindowViewport.h>

static constexpr std::array Vertices
{//                   Position,              UColor,      TexCoords
	KtVertex2D{ {-0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f} }, // Bottom-left
	KtVertex2D{ { 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f} }, // Bottom-right
	KtVertex2D{ { 0.5f,  0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f} }, // Top-right
	KtVertex2D{ {-0.5f,  0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f} }  // Top-left
};

static constexpr std::array Indices{ 0u, 1u, 2u, 2u, 3u, 0u };

void KtInterfaceRenderer::Init()
{
	CreateVertexBuffer();
	CreateIndexBuffer();
	CreateCommandBuffers();
	MarkCommandBuffersDirty();
}

void KtInterfaceRenderer::Update(const u32 frameIndex)
{
	UpdateStagingProxies(frameIndex);
	UpdateProxies(frameIndex);
	DeleteProxies();
}

void KtInterfaceRenderer::Cleanup() const
{
	vmaDestroyBuffer(Context.GetAllocator(), indexBuffer_.Buffer, indexBuffer_.Allocation);
	vmaDestroyBuffer(Context.GetAllocator(), vertexBuffer_.Buffer, vertexBuffer_.Allocation);
	KT_LOG(ELogImportanceLevel::High, "Graphics", "cleaned up interface renderer");
}

void KtInterfaceRenderer::MarkCommandBuffersDirty()
{
	for (auto& frameData : frameDatas_)
	{
		frameData.objectBuffer.isDirty = true;
	}
}

void KtInterfaceRenderer::SetUniformData(const KtInterfaceUniformData& uniformData)
{
	frameDatas_[Renderer.GetGameThreadFrame()].uniformData = uniformData;
}

void KtInterfaceRenderer::RegisterProxy(Proxy* proxy)
{
	stagingProxies_[proxy] = static_cast<i32>(KT_FRAMES_IN_FLIGHT);
}

void KtInterfaceRenderer::UnregisterProxy(Proxy* proxy)
{
	stagingProxies_[proxy] = -static_cast<i32>(KT_FRAMES_IN_FLIGHT);
}

void KtInterfaceRenderer::CmdDraw(VkCommandBuffer commandBuffer, const u32 frameIndex)
{
	auto& frameData{ frameDatas_[frameIndex] };

	KT_LOG(ELogImportanceLevel::Low, "Graphics", "{} proxies", frameData.objectBuffer.proxies.size());

	if (frameData.objectBuffer.isDirty)
	{
		frameData.objectBuffer.isDirty = false;
		frameData.instanceIndices.clear();

		//const KtInterfaceCuller culler{};
		//frameData.sortedProxies = culler.ComputeCulling(frameData.proxies, frameIndex);

		frameData.objectBuffer.sortedProxies = frameData.objectBuffer.proxies;
		SortProxies(frameData.objectBuffer.sortedProxies, frameIndex);
		UpdateDescriptorSets(frameData.objectBuffer.sortedProxies, frameIndex);
		
		UpdateDrawBatches(frameData.objectBuffer, frameIndex);
		RecordCommandBuffer(frameIndex);
	}

	vkCmdExecuteCommands(commandBuffer, 1, &frameData.objectBuffer.commandBuffer);
}

UInterfaceProxy* KtInterfaceRenderer::CreateProxy() const
{
	return new Proxy{};
}

void KtInterfaceRenderer::DeleteProxy(Proxy* proxy)
{
	deleteProxies_[proxy] = static_cast<u32>(KT_FRAMES_IN_FLIGHT);
}

void KtInterfaceRenderer::CreateVertexBuffer()
{
	const VkDeviceSize bufferSize{ sizeof(KtVertex2D) * Vertices.size() };

	Context.CreateBuffer(
		bufferSize,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT,
		stagingVertexBuffer_
	);

	memcpy(stagingVertexBuffer_.AllocationInfo.pMappedData, Vertices.data(), static_cast<size>(bufferSize));

	Context.CreateBuffer(
		bufferSize,
		VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT,
		vertexBuffer_
	);

	Context.CopyBuffer(stagingVertexBuffer_.Buffer, vertexBuffer_.Buffer, bufferSize);
	Context.GetEventExecuteSingleTimeCommands().AddListener(this, &KtInterfaceRenderer::DestroyStagingVertexBuffer);
}

void KtInterfaceRenderer::CreateIndexBuffer()
{
	const VkDeviceSize bufferSize{ sizeof(u32) * Indices.size() };

	Context.CreateBuffer(
		bufferSize,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT,
		stagingIndexBuffer_
	);

	memcpy(stagingIndexBuffer_.AllocationInfo.pMappedData, Indices.data(), static_cast<size>(bufferSize));

	Context.CreateBuffer(
		bufferSize,
		VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT,
		indexBuffer_
	);

	Context.CopyBuffer(stagingIndexBuffer_.Buffer, indexBuffer_.Buffer, bufferSize);
	Context.GetEventExecuteSingleTimeCommands().AddListener(this, &KtInterfaceRenderer::DestroyStagingIndexBuffer);
}

void KtInterfaceRenderer::DestroyStagingVertexBuffer() const
{
	vmaDestroyBuffer(Context.GetAllocator(), stagingVertexBuffer_.Buffer, stagingVertexBuffer_.Allocation);
}

void KtInterfaceRenderer::DestroyStagingIndexBuffer() const
{
	vmaDestroyBuffer(Context.GetAllocator(), stagingIndexBuffer_.Buffer, stagingIndexBuffer_.Allocation);
}

void KtInterfaceRenderer::CmdBindVertexBuffer(VkCommandBuffer commandBuffer) const
{
	const std::array vertexBuffers{ vertexBuffer_.Buffer };
	const std::array offsets{ VkDeviceSize{ 0 } };
	vkCmdBindVertexBuffers(commandBuffer, 0, static_cast<u32>(vertexBuffers.size()), vertexBuffers.data(), offsets.data());
}

void KtInterfaceRenderer::CmdBindIndexBuffer(VkCommandBuffer commandBuffer) const
{
	vkCmdBindIndexBuffer(commandBuffer, indexBuffer_.Buffer, 0, VK_INDEX_TYPE_UINT32);
}

void KtInterfaceRenderer::CreateCommandBuffers()
{
	for (size i{ 0 }; i < KT_FRAMES_IN_FLIGHT; ++i)
	{
		CreateCommandBuffer(static_cast<u32>(i));
	}
}

void KtInterfaceRenderer::CreateCommandBuffer(const u32 frameIndex)
{
	const VkCommandBufferAllocateInfo allocInfo{
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
		.commandPool = Renderer.GetCommandPool(frameIndex),
		.level = VK_COMMAND_BUFFER_LEVEL_SECONDARY,
		.commandBufferCount = 1,
	};

	VK_CHECK_THROW(
		vkAllocateCommandBuffers(Context.GetDevice(), &allocInfo, &frameDatas_[frameIndex].objectBuffer.commandBuffer),
		"failed to allocate command buffers!"
	);
}

void KtInterfaceRenderer::RecordCommandBuffer(const u32 frameIndex)
{
	VkCommandBuffer commandBuffer{ frameDatas_[frameIndex].objectBuffer.commandBuffer };
	BeginCommandBuffer(commandBuffer, frameIndex);
	CmdDrawProxies(commandBuffer, frameDatas_[frameIndex].objectBuffer.drawBatches, frameIndex);
	EndCommandBuffer(commandBuffer);
}

void KtInterfaceRenderer::BeginCommandBuffer(VkCommandBuffer commandBuffer, const u32 frameIndex)
{
	vkResetCommandBuffer(commandBuffer, 0);

	const std::array colorAttachmentFormats{ Renderer.GetSwapChainFormat() };
	const VkCommandBufferInheritanceRenderingInfo inheritanceRenderingInfo{
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_RENDERING_INFO,
		.pNext = VK_NULL_HANDLE,
		.flags = 0,
		.colorAttachmentCount = static_cast<u32>(colorAttachmentFormats.size()),
		.pColorAttachmentFormats = colorAttachmentFormats.data(),
		.depthAttachmentFormat = Renderer.GetDepthFormat(),
		.stencilAttachmentFormat = VK_FORMAT_UNDEFINED, // Change if using stencil
		.rasterizationSamples = Context.GetMSAASamples()
	};

	const VkCommandBufferInheritanceInfo inheritanceInfo{
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO,
		.pNext = &inheritanceRenderingInfo,   
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

void KtInterfaceRenderer::UpdateProxies(const u32 frameIndex)
{
	for (Proxy* proxy : frameDatas_[frameIndex].objectBuffer.proxies)
	{
		if (proxy->IsDirty())
		{
			frameDatas_[frameIndex].objectBuffer.isDirty = true;
			proxy->ApplyPendingUpdates(frameIndex);
		}
	}
}

void KtInterfaceRenderer::UpdateStagingProxies(const u32 frameIndex)
{
	if (stagingProxies_.empty())
	{
		return;
	}

	for (auto& [proxy, count] : stagingProxies_)
	{
		if (count == 0)
		{
			continue;
		}

		frameDatas_[frameIndex].objectBuffer.isDirty = true;
		KT_LOG(ELogImportanceLevel::Medium, "Graphics", "dirty command buffer frame {}", frameIndex);

		if (count > 0)
		{
			if (!proxy->frameDatas_[frameIndex].poolData.isRegistered)
			{
				proxy->frameDatas_[frameIndex].poolData.isRegistered = true;
				frameDatas_[frameIndex].objectBuffer.proxies.Add(proxy);
				proxy->frameDatas_[frameIndex].poolData.index = frameDatas_[frameIndex].objectBuffer.proxies.LastIndex();
			}

			--count;
		}
		else if (count < 0)
		{
			if (proxy->frameDatas_[frameIndex].poolData.isRegistered)
			{
				proxy->frameDatas_[frameIndex].poolData.isRegistered = false;
				const size index{ proxy->frameDatas_[frameIndex].poolData.index };
				if (frameDatas_[frameIndex].objectBuffer.proxies.RemoveAt(index) == KtPoolRemoveResult::ItemSwappedAndRemoved)
				{
					frameDatas_[frameIndex].objectBuffer.proxies[index]->frameDatas_[frameIndex].poolData.index = index;
				}
			}

			++count;
		}
	}

	std::erase_if(stagingProxies_,
		[](const std::pair<const Proxy*, i32>& pair)
		{
			return pair.second == 0;
		}
	);
}

void KtInterfaceRenderer::UpdateDescriptorSets(const ProxiesPool& proxies, const u32 frameIndex)
{
	struct ShaderData
	{
		std::vector<KtInterfaceObjectData> objectBufferDatas;
		std::vector<KtInterfaceRenderable*> renderables;
		std::vector<u32> renderableIndices;
	};

	std::unordered_map<KtShader*, ShaderData> shaderDatas{};

	for (const auto* proxy : proxies)
	{
		auto& frameData{ proxy->frameDatas_[frameIndex] };
		auto& shaderData{ shaderDatas[frameData.data.shader] };

		shaderData.objectBufferDatas.push_back(frameData.data.objectData);

		const auto it{ std::find(shaderData.renderables.begin(), shaderData.renderables.end(), frameData.data.renderable) };

		size index;
		if (it == shaderData.renderables.end())
		{
			index = shaderData.renderables.size();
			shaderData.renderables.push_back(frameData.data.renderable);
		}
		else
		{
			index = std::distance(shaderData.renderables.begin(), it);
		}

		shaderData.renderableIndices.push_back(static_cast<u32>(index));
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
				// static_cast is safe because 'textures' expects only KtTexture
				const auto* asTexture{ static_cast<const KtTexture*>(renderable) };
				imageInfos.push_back(asTexture->GetDescriptorImageInfo());
			}
			shader->UpdateDescriptorSetLayoutBindingImageSampler(*binding, imageInfos, frameIndex);
		}
	}
}

void KtInterfaceRenderer::DeleteProxies()
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
		[](const std::pair<const Proxy*, u32>& pair)
		{
			return pair.second == 0;
		}
	);
}

void KtInterfaceRenderer::SortProxies(ProxiesPool& proxies, const u32 frameIndex)
{
	std::sort(proxies.begin(), proxies.end(),
		[frameIndex](const Proxy* a, const Proxy* b)
		{
			const auto& aFrameData{ a->frameDatas_[frameIndex] };
			const auto& bFrameData{ b->frameDatas_[frameIndex] };
			if (aFrameData.data.layer != bFrameData.data.layer)
			{
				return aFrameData.data.layer < bFrameData.data.layer;
			}
			if (aFrameData.data.shader != bFrameData.data.shader)
			{
				return aFrameData.data.shader < bFrameData.data.shader;
			}
			return aFrameData.data.renderable < bFrameData.data.renderable;
		}
	);
}

void KtInterfaceRenderer::CmdDrawProxies(VkCommandBuffer commandBuffer, const std::vector<DrawBatch>& drawBatches, const u32 frameIndex)
{
	if (drawBatches.empty())
	{
		return;
	}

	WindowViewport.CmdUse(commandBuffer);
	CmdBindVertexBuffer(commandBuffer);
	CmdBindIndexBuffer(commandBuffer);

	const KtShader* currentShader{ nullptr };
	KtScissor currentScissor{};

	for (auto& drawBatch : drawBatches)
	{
		if (currentShader != drawBatch.shader)
		{
			currentShader = drawBatch.shader;
			currentShader->CmdBind(commandBuffer);
			currentShader->CmdBindDescriptorSets(commandBuffer, frameIndex);
		}

		if (currentScissor != drawBatch.scissor)
		{
			currentScissor = drawBatch.scissor;
			const auto offset{ drawBatch.scissor.offset };
			const auto extent{ drawBatch.scissor.extent };
			const VkRect2D vkScissor{
				.offset = { offset.x, offset.y },
				.extent = { extent.x, extent.y },
			};
			vkCmdSetScissor(commandBuffer, 0, 1, &vkScissor);
		}

		vkCmdDrawIndexed(commandBuffer, static_cast<u32>(Indices.size()), drawBatch.instanceCount, 0, 0, drawBatch.firstInstance);
	}
}

void KtInterfaceRenderer::UpdateDrawBatches(FrameData::ObjectBufferData& objectBuffer, const u32 frameIndex)
{
	const auto& proxies{ objectBuffer.sortedProxies };

	objectBuffer.drawBatches.clear();
	objectBuffer.drawBatches.reserve(proxies.size());

	for (size i{ 0 }; i < proxies.size();)
	{
		const auto& frameData{ proxies[i]->frameDatas_[frameIndex] };
		KtShader* shader{ frameData.data.shader };
		KtScissor scissor{ frameData.data.scissor };

		// Find the extent of the current batch
		size instanceCount{ 1 };
		for (; i + instanceCount < proxies.size(); ++instanceCount)
		{
			const auto& nextFrameData{ proxies[i + instanceCount]->frameDatas_[frameIndex] };
			if (nextFrameData.data.shader != shader ||
				nextFrameData.data.scissor != scissor)
			{
				break;
			}
		}

		const DrawBatch batch{
			.shader = shader,
			.scissor = scissor,
			.firstInstance = frameDatas_[frameIndex].instanceIndices[shader],
			.instanceCount = static_cast<u32>(instanceCount),
		};
		objectBuffer.drawBatches.push_back(batch);

		frameDatas_[frameIndex].instanceIndices[shader] += static_cast<u32>(instanceCount);
		i += instanceCount;
	}
}
