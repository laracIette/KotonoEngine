#include "Renderer2D.h"
#include "Framework.h"
#include "log.h"
#include "Context.h"
#include "Vertex2D.h"

constexpr std::array<KtVertex2D, 4> SquareVertices =
{//                   Position,              Color,      TexCoords
	KtVertex2D { {-0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f} }, // Bottom-left
	KtVertex2D { { 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f} }, // Bottom-right
	KtVertex2D { { 0.5f,  0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f} }, // Top-right
	KtVertex2D { {-0.5f,  0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f} }  // Top-left
};
constexpr std::array<uint32_t, 6> SquareIndices = { 0, 1, 2, 2, 3, 0 };

void KtRenderer2D::Init()
{
	CreateVertexBuffer();
	CreateIndexBuffer();
}

void KtRenderer2D::Cleanup() const
{
	KT_DEBUG_LOG("cleaning up 2D renderer");
	vmaDestroyBuffer(Framework.GetContext().GetAllocator(), _indexBuffer.Buffer, _indexBuffer.Allocation);
	vmaDestroyBuffer(Framework.GetContext().GetAllocator(), _vertexBuffer.Buffer, _vertexBuffer.Allocation);
	KT_DEBUG_LOG("cleaned up 2D renderer");
}

void KtRenderer2D::AddToRenderQueue(const KtAddToRenderQueue2DArgs& args)
{
	_renderQueueData[Framework.GetRenderer().GetGameThreadFrame()]
		.Shaders[args.Shader]
		.Renderables[args.Renderable]
		.Viewports[args.Viewport]
		.Layers[args.Layer]
		.ObjectDatas.push_back(args.ObjectData);
}

void KtRenderer2D::SetUniformData(const KtUniformData2D& uniformData)
{
	_uniformData[Framework.GetRenderer().GetGameThreadFrame()] = uniformData;
}

void KtRenderer2D::CmdDraw(VkCommandBuffer commandBuffer, const uint32_t currentFrame) const
{
	const auto culledData = _culler.ComputeCulling(_renderQueueData[currentFrame]);

	for (auto& [shader, shaderData] : culledData.Shaders)
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
					objectBufferData.insert(objectBufferData.end(),
						layerData.ObjectDatas.begin(), layerData.ObjectDatas.end()
					);
					renderableIndices.push_back(static_cast<uint32_t>(renderables.size()));
				}
			}
			renderables.push_back(renderable);
		}
		
		// NOT A CMD, UPDATE ONCE PER FRAME //
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
			std::vector<VkDescriptorImageInfo> imageInfos{};
			imageInfos.reserve(renderables.size());
			for (auto* renderable : renderables)
			{
				if (auto* imageTexture = dynamic_cast<KtImageTexture*>(renderable))
				{
					imageInfos.push_back(imageTexture->GetDescriptorImageInfo());
				}
			}
			shader->UpdateDescriptorSetLayoutBindingImageSampler(*binding, imageInfos[0], currentFrame);
		}
		// -------------------------------- //

		shader->CmdBind(commandBuffer);
		shader->CmdBindDescriptorSets(commandBuffer, currentFrame);

		CmdBindBuffers(commandBuffer);

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

				viewport->CmdUse(commandBuffer);
				vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(SquareIndices.size()), instanceCount, 0, 0, instanceIndex);

				instanceIndex += instanceCount;
			}
		}
	}
}

void KtRenderer2D::CmdBindBuffers(VkCommandBuffer commandBuffer) const
{
	const std::array<VkBuffer, 1> vertexBuffers = { _vertexBuffer.Buffer };
	const std::array<VkDeviceSize, 1> offsets = { 0 };
	vkCmdBindVertexBuffers(commandBuffer, 0, static_cast<uint32_t>(vertexBuffers.size()), vertexBuffers.data(), offsets.data());
	vkCmdBindIndexBuffer(commandBuffer, _indexBuffer.Buffer, 0, VK_INDEX_TYPE_UINT32);
}

void KtRenderer2D::Reset(const uint32_t currentFrame)
{
	_uniformData[currentFrame] = {};
	_renderQueueData[currentFrame] = {};
}

void KtRenderer2D::CreateVertexBuffer()
{
	const VkDeviceSize bufferSize = sizeof(KtVertex2D) * SquareVertices.size();

	KtAllocatedBuffer stagingBuffer;
	Framework.GetContext().CreateBuffer(
		bufferSize,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT,
		stagingBuffer
	);

	memcpy(stagingBuffer.AllocationInfo.pMappedData, SquareVertices.data(), (size_t)bufferSize);

	Framework.GetContext().CreateBuffer(
		bufferSize,
		VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT,
		_vertexBuffer
	);

	Framework.GetContext().CopyBuffer(stagingBuffer.Buffer, _vertexBuffer.Buffer, bufferSize);

	vmaDestroyBuffer(Framework.GetContext().GetAllocator(), stagingBuffer.Buffer, stagingBuffer.Allocation);
}

void KtRenderer2D::CreateIndexBuffer()
{
	const VkDeviceSize bufferSize = sizeof(uint32_t) * SquareIndices.size();

	KtAllocatedBuffer stagingBuffer;
	Framework.GetContext().CreateBuffer(
		bufferSize,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT,
		stagingBuffer
	);

	memcpy(stagingBuffer.AllocationInfo.pMappedData, SquareIndices.data(), (size_t)bufferSize);

	Framework.GetContext().CreateBuffer(
		bufferSize,
		VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT,
		_indexBuffer
	);

	Framework.GetContext().CopyBuffer(stagingBuffer.Buffer, _indexBuffer.Buffer, bufferSize);

	vmaDestroyBuffer(Framework.GetContext().GetAllocator(), stagingBuffer.Buffer, stagingBuffer.Allocation);
}
