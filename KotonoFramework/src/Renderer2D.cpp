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
}

void KtRenderer2D::Cleanup() const
{
	KT_DEBUG_LOG("cleaning up 2D renderer");
	vmaDestroyBuffer(Framework.GetContext().GetAllocator(), indexBuffer_.Buffer, indexBuffer_.Allocation);
	vmaDestroyBuffer(Framework.GetContext().GetAllocator(), vertexBuffer_.Buffer, vertexBuffer_.Allocation);
	KT_DEBUG_LOG("cleaned up 2D renderer");
}

void KtRenderer2D::AddToRenderQueue(const KtAddToRenderQueue2DArgs& args)
{
	renderQueueData_[Framework.GetRenderer().GetGameThreadFrame()]
		.Shaders[args.Shader]
		.Renderables[args.Renderable]
		.Viewports[args.Viewport]
		.Layers[args.Layer] // todo: change to unordered_map and sort in culler or sorter
		.ObjectDatas.push_back(args.ObjectData);
}

void KtRenderer2D::SetUniformData(const KtUniformData2D& uniformData)
{
	uniformData_[Framework.GetRenderer().GetGameThreadFrame()] = uniformData;
}

void KtRenderer2D::CmdDraw(VkCommandBuffer commandBuffer, const uint32_t currentFrame) const
{
	const KtCuller2D culler{};
	const auto culledData = culler.ComputeCulling(renderQueueData_[currentFrame]);
	CmdDrawRenderQueue(commandBuffer, culledData, currentFrame);
}

void KtRenderer2D::CmdDrawRenderQueue(VkCommandBuffer commandBuffer, const KtRenderQueue2DData& renderQueueData, const uint32_t currentFrame) const
{
	KtViewport* currentViewport = nullptr;

	for (auto& [shader, shaderData] : renderQueueData.Shaders)
	{
		std::vector<KtObjectData2D> objectBufferData;
		std::vector<KtRenderable2D*> renderables;
		renderables.reserve(shaderData.Renderables.size());
		std::vector<uint32_t> renderableIndices;
		for (const auto& [renderable, renderableData] : shaderData.Renderables)
		{
			for (const auto& [viewport, viewportData] : renderableData.Viewports)
			{
				for (const auto& [layer, layerData] : viewportData.Layers)
				{
					// insert layerData.ObjectDatas.begin() to layerData.ObjectDatas.end()
					objectBufferData.insert(objectBufferData.end(),
						layerData.ObjectDatas.begin(), layerData.ObjectDatas.end()
					);

					// insert layerData.ObjectDatas.size(), renderables.size() times
					renderableIndices.insert(renderableIndices.end(),
						layerData.ObjectDatas.size(), static_cast<uint32_t>(renderables.size())
					);
				}
			}
			renderables.push_back(renderable);
		}

		// NOT A CMD, UPDATE ONCE PER FRAME PER SHADER //
		if (auto* binding = shader->GetDescriptorSetLayoutBinding("objectBuffer"))
		{
			shader->UpdateDescriptorSetLayoutBindingBufferMemberCount(*binding, objectBufferData.size(), currentFrame);
			shader->UpdateDescriptorSetLayoutBindingBuffer(*binding, objectBufferData.data(), currentFrame);
		}
		if (auto* binding = shader->GetDescriptorSetLayoutBinding("textureIndexBuffer"))
		{
			shader->UpdateDescriptorSetLayoutBindingBufferMemberCount(*binding, renderableIndices.size(), currentFrame);
			shader->UpdateDescriptorSetLayoutBindingBuffer(*binding, renderableIndices.data(), currentFrame);
		}
		if (auto* binding = shader->GetDescriptorSetLayoutBinding("textures"))
		{
			std::vector<VkDescriptorImageInfo> imageInfos;
			imageInfos.reserve(renderables.size());
			for (const auto* renderable : renderables)
			{
				// static_cast is safe because 'textures' expects only KtImageTexture
				const auto* imageTexture = static_cast<const KtImageTexture*>(renderable);
				imageInfos.push_back(imageTexture->GetDescriptorImageInfo());
			}
			shader->UpdateDescriptorSetLayoutBindingImageSampler(*binding, imageInfos, currentFrame);
		}
		// ------------------------------------------- //

		shader->CmdBind(commandBuffer);
		shader->CmdBindDescriptorSets(commandBuffer, currentFrame);

		CmdBindVertexBuffer(commandBuffer);
		CmdBindIndexBuffer(commandBuffer);

		uint32_t instanceIndex = 0;
		for (const auto& [renderable, renderableData] : shaderData.Renderables)
		{
			for (const auto& [viewport, viewportData] : renderableData.Viewports)
			{
				uint32_t instanceCount = 0;
				for (const auto& [layer, layerData] : viewportData.Layers)
				{
					instanceCount += static_cast<uint32_t>(layerData.ObjectDatas.size());
				}

				if (currentViewport != viewport)
				{
					currentViewport = viewport;
					currentViewport->CmdUse(commandBuffer);
				}

				vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(Indices.size()), instanceCount, 0, 0, instanceIndex);
				instanceIndex += instanceCount;
			}
		}
	}
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

void KtRenderer2D::Reset(const uint32_t currentFrame)
{
	uniformData_[currentFrame] = {};
	renderQueueData_[currentFrame] = {};
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
