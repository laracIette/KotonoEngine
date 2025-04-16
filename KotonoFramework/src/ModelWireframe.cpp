#include "ModelWireframe.h"
#include "Framework.h"
#include "Context.h"
#include "log.h"

void KtModelWireframe::Init(const std::span<KtVertex3D> vertices, const std::span<uint32_t> indices)
{
	for (const auto& vertex : vertices)
	{
		_vertices.push_back(vertex.Position);
	}
	_indices = std::vector<uint32_t>(indices.begin(), indices.end());

	CreateVertexBuffer();
	CreateIndexBuffer();

	_isInit = true;
}

void KtModelWireframe::Cleanup() const
{
	KT_DEBUG_LOG("cleaning up model wireframe");
	vmaDestroyBuffer(Framework.GetContext().GetAllocator(), _indexBuffer.Buffer, _indexBuffer.Allocation);
	vmaDestroyBuffer(Framework.GetContext().GetAllocator(), _vertexBuffer.Buffer, _vertexBuffer.Allocation);
	KT_DEBUG_LOG("cleaned up model wireframe");
}

const bool KtModelWireframe::GetIsInit() const
{
	return _isInit;
}

void KtModelWireframe::CmdBind(VkCommandBuffer commandBuffer) const
{
	const std::array<VkBuffer, 1> vertexBuffers = { _vertexBuffer.Buffer };
	const std::array<VkDeviceSize, 1> offsets = { 0 };
	vkCmdBindVertexBuffers(commandBuffer, 0, static_cast<uint32_t>(vertexBuffers.size()), vertexBuffers.data(), offsets.data());
	vkCmdBindIndexBuffer(commandBuffer, _indexBuffer.Buffer, 0, VK_INDEX_TYPE_UINT32);
}

void KtModelWireframe::CmdDraw(VkCommandBuffer commandBuffer, const uint32_t instanceCount, const uint32_t firstInstance) const
{
	vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(_indices.size()), instanceCount, 0, 0, firstInstance);
}

void KtModelWireframe::CreateVertexBuffer()
{
	const VkDeviceSize bufferSize = sizeof(glm::vec3) * _vertices.size();

	KtAllocatedBuffer stagingBuffer;
	Framework.GetContext().CreateBuffer(
		bufferSize,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT,
		stagingBuffer
	);

	memcpy(stagingBuffer.AllocationInfo.pMappedData, _vertices.data(), (size_t)bufferSize);

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

void KtModelWireframe::CreateIndexBuffer()
{
	const VkDeviceSize bufferSize = sizeof(uint32_t) * _indices.size();

	KtAllocatedBuffer stagingBuffer;
	Framework.GetContext().CreateBuffer(
		bufferSize,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT,
		stagingBuffer
	);

	memcpy(stagingBuffer.AllocationInfo.pMappedData, _indices.data(), (size_t)bufferSize);

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
