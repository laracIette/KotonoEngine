#include "Renderer2D.h"
#include "log.h"
#include "Framework.h"
#include "Vertex2D.h"

constexpr std::array<KtVertex2D, 4> SquareVertices =
{//                   Position,              Color,      TexCoords
	KtVertex2D{ {-0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f} }, // Bottom-left
	KtVertex2D{ { 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f} }, // Bottom-right
	KtVertex2D{ { 0.5f,  0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f} }, // Top-right
	KtVertex2D{ {-0.5f,  0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f} }  // Top-left
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
	_renderQueue2DData
		.Shaders[args.Shader]
		.Viewports[args.Viewport]
		.ObjectDatas.push_back(args.ObjectData);
}

void KtRenderer2D::SetUniformData(const KtUniformData2D& uniformData)
{
	_uniformData = uniformData;
}

void KtRenderer2D::CmdDraw(VkCommandBuffer commandBuffer, const uint32_t currentFrame) const
{
	for (auto& [shader, shaderData] : _renderQueue2DData.Shaders)
	{
		if (!shader)
		{
			KT_DEBUG_LOG("shader is nullptr");
			continue;
		}

		std::vector<KtObjectData2D> objectBufferData;
		for (auto& [viewport, viewportData] : shaderData.Viewports)
		{
			objectBufferData.insert(objectBufferData.end(),
				viewportData.ObjectDatas.begin(), viewportData.ObjectDatas.end()
			);
		}
		// NOT A CMD, UPDATE ONCE PER FRAME //
		if (auto* binding = shader->GetDescriptorSetLayoutBinding("objectBuffer"))
		{
			shader->UpdateDescriptorSetLayoutBindingMemberCount(*binding, objectBufferData.size(), currentFrame);
			shader->UpdateDescriptorSetLayoutBindingBuffer(*binding, objectBufferData.data(), currentFrame);
		}
		// -------------------------------- //

		shader->CmdBind(commandBuffer);
		shader->CmdBindDescriptorSets(commandBuffer, currentFrame);

		CmdBindBuffers(commandBuffer);

		uint32_t instanceIndex = 0;
		for (auto& [viewport, viewportData] : shaderData.Viewports)
		{
			const uint32_t instanceCount = static_cast<uint32_t>(viewportData.ObjectDatas.size());

			if (!viewport)
			{
				KT_DEBUG_LOG("viewport is nullptr");
			}
			else if (instanceCount == 0)
			{
				KT_DEBUG_LOG("model doesn't have any instance");
			}
			else
			{
				viewport->CmdUse(commandBuffer);
				vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(SquareIndices.size()), instanceCount, 0, 0, instanceIndex);
			}

			instanceIndex += instanceCount;
		}
	}
}

void KtRenderer2D::CmdBindBuffers(VkCommandBuffer commandBuffer) const
{
	const VkBuffer vertexBuffers[] = { _vertexBuffer.Buffer };
	const VkDeviceSize offsets[] = { 0 };
	vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
	vkCmdBindIndexBuffer(commandBuffer, _indexBuffer.Buffer, 0, VK_INDEX_TYPE_UINT32);
}

void KtRenderer2D::Reset()
{
	_uniformData = {};
	_renderQueue2DData = {};
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
