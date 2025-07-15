#include "Renderer2D.h"
#include "Framework.h"
#include "log.h"
#include "Context.h"
#include "Vertex2D.h"
#include "Culler2D.h"
#include "Renderer.h"
#include "ImageTexture.h"
#include "Shader.h"
#include "Viewport.h"
#include "vk_utils.h"
#include "Renderable2DProxy.h"
#include "Collection.h"

static constexpr std::array<KtVertex2D, 4> Vertices =
{//                   Position,              Color,      TexCoords
	KtVertex2D { {-0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f} }, // Bottom-left
	KtVertex2D { { 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f} }, // Bottom-right
	KtVertex2D { { 0.5f,  0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f} }, // Top-right
	KtVertex2D { {-0.5f,  0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f} }  // Top-left
};

static constexpr std::array<uint32_t, 6> Indices = { 0, 1, 2, 2, 3, 0 };

void KtRenderer2D::Init()
{
	CreateVertexBuffer();
	CreateIndexBuffer();
	CreateCommandBuffers();
	isCommandBufferDirty_.fill(true);
}

void KtRenderer2D::Update(const uint32_t frameIndex)
{
	for (auto& [proxy, count] : stagingProxies_)
	{
		if (count == 0)
		{
			continue;
		}

		isCommandBufferDirty_[frameIndex] = true;

		if (count > 0)
		{
			proxies_[frameIndex].insert(proxy);
			--count;
		}
		else if (count < 0)
		{
			proxies_[frameIndex].erase(proxy);
			++count;
		}
	}

	std::erase_if(stagingProxies_,
		[](const std::pair<KtRenderable2DProxy*, int32_t>& pair)
		{
			return pair.second == 0;
		}
	);
}

void KtRenderer2D::Cleanup() const
{
	KT_LOG_KF(KT_LOG_IMPORTANCE_LEVEL_HIGH, "cleaning up 2D renderer");
	vmaDestroyBuffer(Framework.GetContext().GetAllocator(), indexBuffer_.Buffer, indexBuffer_.Allocation);
	vmaDestroyBuffer(Framework.GetContext().GetAllocator(), vertexBuffer_.Buffer, vertexBuffer_.Allocation);
	KT_LOG_KF(KT_LOG_IMPORTANCE_LEVEL_HIGH, "cleaned up 2D renderer");
}

void KtRenderer2D::CreateVertexBuffer()
{
	const VkDeviceSize bufferSize = sizeof(KtVertex2D) * Vertices.size();

	KtAllocatedBuffer stagingBuffer;
	Framework.GetContext().CreateBuffer(
		bufferSize,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT,
		stagingBuffer
	);

	memcpy(stagingBuffer.AllocationInfo.pMappedData, Vertices.data(), (size_t)bufferSize);

	Framework.GetContext().CreateBuffer(
		bufferSize,
		VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT,
		vertexBuffer_
	);

	Framework.GetContext().CopyBuffer(stagingBuffer.Buffer, vertexBuffer_.Buffer, bufferSize);

	vmaDestroyBuffer(Framework.GetContext().GetAllocator(), stagingBuffer.Buffer, stagingBuffer.Allocation);
}

void KtRenderer2D::CreateIndexBuffer()
{
	const VkDeviceSize bufferSize = sizeof(uint32_t) * Indices.size();

	KtAllocatedBuffer stagingBuffer;
	Framework.GetContext().CreateBuffer(
		bufferSize,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT,
		stagingBuffer
	);

	memcpy(stagingBuffer.AllocationInfo.pMappedData, Indices.data(), (size_t)bufferSize);

	Framework.GetContext().CreateBuffer(
		bufferSize,
		VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT,
		indexBuffer_
	);

	Framework.GetContext().CopyBuffer(stagingBuffer.Buffer, indexBuffer_.Buffer, bufferSize);

	vmaDestroyBuffer(Framework.GetContext().GetAllocator(), stagingBuffer.Buffer, stagingBuffer.Allocation);
}

void KtRenderer2D::CmdBindVertexBuffer(VkCommandBuffer commandBuffer) const
{
	const std::array<VkBuffer, 1> vertexBuffers = { vertexBuffer_.Buffer };
	const std::array<VkDeviceSize, 1> offsets = { 0 };
	vkCmdBindVertexBuffers(commandBuffer, 0, static_cast<uint32_t>(vertexBuffers.size()), vertexBuffers.data(), offsets.data());
}

void KtRenderer2D::CmdBindIndexBuffer(VkCommandBuffer commandBuffer) const
{
	vkCmdBindIndexBuffer(commandBuffer, indexBuffer_.Buffer, 0, VK_INDEX_TYPE_UINT32);
}

void KtRenderer2D::CreateCommandBuffers()
{
	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = Framework.GetContext().GetCommandPool();
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_SECONDARY;
	allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers_.size());

	VK_CHECK_THROW(
		vkAllocateCommandBuffers(Framework.GetContext().GetDevice(), &allocInfo, commandBuffers_.data()),
		"failed to allocate command buffers!"
	);
}

void KtRenderer2D::RecordCommandBuffer(const uint32_t frameIndex)
{
	VkCommandBuffer commandBuffer = commandBuffers_[frameIndex];
	BeginCommandBuffer(commandBuffer, frameIndex);
	CmdDrawProxies(commandBuffer, sortedProxies_[frameIndex], frameIndex);
	EndCommandBuffer(commandBuffer);
}

void KtRenderer2D::BeginCommandBuffer(VkCommandBuffer commandBuffer, const uint32_t frameIndex)
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

void KtRenderer2D::EndCommandBuffer(VkCommandBuffer commandBuffer)
{
	VK_CHECK_THROW(
		vkEndCommandBuffer(commandBuffer),
		"failed to record command buffer!"
	);
}

void KtRenderer2D::SetUniformData(const KtUniformData2D& uniformData)
{
	uniformDatas_[Framework.GetRenderer().GetGameThreadFrame()] = uniformData;
}

void KtRenderer2D::Register(KtRenderable2DProxy* proxy)
{
	stagingProxies_[proxy] = static_cast<int32_t>(KT_FRAMES_IN_FLIGHT);
}

void KtRenderer2D::Remove(KtRenderable2DProxy* proxy)
{
	stagingProxies_[proxy] = -static_cast<int32_t>(KT_FRAMES_IN_FLIGHT);
}

void KtRenderer2D::CmdDraw(VkCommandBuffer commandBuffer, const uint32_t frameIndex)
{

	if (isCommandBufferDirty_[frameIndex] || GetIsAnyProxyDirty(frameIndex))
	{
		isCommandBufferDirty_[frameIndex] = false;

		const KtCuller2D culler{};
		const auto culledData = culler.ComputeCulling(proxies_[frameIndex]);
		sortedProxies_[frameIndex] = GetSortedProxies(culledData);

		instanceIndices_[frameIndex] = {};
		UpdateDescriptorSets(sortedProxies_[frameIndex], frameIndex);

		RecordCommandBuffer(frameIndex);
		MarkProxiesNotDirty(frameIndex);
	}

	vkCmdExecuteCommands(commandBuffer, 1, &commandBuffers_[frameIndex]);
}

void KtRenderer2D::UpdateDescriptorSets(const ProxiesVector& proxies, const uint32_t frameIndex)
{
	struct ShaderData final
	{
		std::vector<KtObjectData2D> objectBufferDatas;
		std::vector<KtRenderable2D*> renderables;
		std::vector<uint32_t> renderableIndices;
	};

	std::unordered_map<KtShader*, ShaderData> shaderDatas;

	for (const auto* proxy : proxies)
	{
		auto& shaderData = shaderDatas[proxy->shader];

		shaderData.objectBufferDatas.push_back(proxy->objectData);

		const auto it = std::find(shaderData.renderables.begin(), shaderData.renderables.end(), proxy->renderable);

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
		if (auto* binding = shader->GetDescriptorSetLayoutBinding("objectBuffer"))
		{
			shader->UpdateDescriptorSetLayoutBindingBufferMemberCount(*binding, shaderData.objectBufferDatas.size(), frameIndex);
			shader->UpdateDescriptorSetLayoutBindingBuffer(*binding, shaderData.objectBufferDatas.data(), frameIndex);
		}

		if (auto* binding = shader->GetDescriptorSetLayoutBinding("textureIndexBuffer"))
		{
			shader->UpdateDescriptorSetLayoutBindingBufferMemberCount(*binding, shaderData.renderableIndices.size(), frameIndex);
			shader->UpdateDescriptorSetLayoutBindingBuffer(*binding, shaderData.renderableIndices.data(), frameIndex);
		}

		if (auto* binding = shader->GetDescriptorSetLayoutBinding("textures"))
		{
			std::vector<VkDescriptorImageInfo> imageInfos;
			imageInfos.reserve(shaderData.renderables.size());
			for (const auto* renderable : shaderData.renderables)
			{
				// static_cast is safe because 'textures' expects only KtImageTexture
				const auto* imageTexture = static_cast<const KtImageTexture*>(renderable);
				imageInfos.push_back(imageTexture->GetDescriptorImageInfo());
			}
			shader->UpdateDescriptorSetLayoutBindingImageSampler(*binding, imageInfos, frameIndex);
		}
	}
}

void KtRenderer2D::CmdDrawProxies(VkCommandBuffer commandBuffer, const ProxiesVector& proxies, const uint32_t frameIndex)
{
	const KtShader* currentShader = nullptr;
	const KtViewport* currentViewport = nullptr;

	CmdBindVertexBuffer(commandBuffer);
	CmdBindIndexBuffer(commandBuffer);

	for (size_t i = 0; i < proxies.size();)
	{
		const auto* proxy = proxies[i];
		const KtShader* shader = proxy->shader;
		const KtViewport* viewport = proxy->viewport;
		const int32_t layer = proxy->layer;

		size_t instanceCount = 1;
		while (i + instanceCount < proxies.size())
		{
			const auto* next = proxies[i + instanceCount];
			if (next->shader != shader || next->viewport != viewport)
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

		if (currentViewport != viewport)
		{
			currentViewport = viewport;
			currentViewport->CmdUse(commandBuffer);
		}
		
		vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(Indices.size()), static_cast<uint32_t>(instanceCount), 0, 0, instanceIndices_[frameIndex][shader]);
		instanceIndices_[frameIndex][shader] += static_cast<uint32_t>(instanceCount);

		i += instanceCount;
	}
}

const KtRenderer2D::ProxiesVector KtRenderer2D::GetSortedProxies(const ProxiesUnorderedSet& proxies) const
{
	ProxiesVector sortedProxies(proxies.begin(), proxies.end());

	std::sort(sortedProxies.begin(), sortedProxies.end(),
		[](const KtRenderable2DProxy* a, const KtRenderable2DProxy* b)
		{
			if (a->shader != b->shader)
			{
				return a->shader < b->shader;
			}
			if (a->renderable != b->renderable)
			{
				return a->renderable < b->renderable;
			}
			if (a->layer != b->layer)
			{
				return a->layer < b->layer;
			}
			return a->viewport < b->viewport;
		}
	);

	return sortedProxies;
}

const bool KtRenderer2D::GetIsAnyProxyDirty(const uint32_t frameIndex) const
{
	auto proxies = KtCollection(proxies_[frameIndex].begin(), proxies_[frameIndex].end());
	proxies.AddFilter([](const KtRenderable2DProxy* proxy) { return proxy->isDirty; });
	return proxies.GetFirst() != nullptr;
}

void KtRenderer2D::MarkProxiesNotDirty(const uint32_t frameIndex)
{
	for (auto* proxy : proxies_[frameIndex])
	{
		proxy->isDirty = false;
	}
}
