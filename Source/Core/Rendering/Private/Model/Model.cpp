#include "Model.h"

#include <Device/Device.h>
#include <MeshLoader/MeshLoader.h>

AModel::AModel(UPath const& path) 
	: AAsset(path)
{
	Load();
}

void AModel::Init(UDevice& device)
{
	CreateVertexBuffer(device);
}

void AModel::Cleanup(UDevice& device) const
{
	device.CleanupAllocatedBuffer(vertexBuffer_);
}

auto AModel::GetVertexBufferAddress() const -> VkDeviceAddress
{
	return vertexBuffer_.bda;
}

auto AModel::GetIndexCount() const -> u32
{
	return meshData_.indices.size();
}

auto AModel::GetFirstIndex() const -> u32
{
	return firstIndex_;
}

void AModel::SetFirstIndex(u32 index)
{
	firstIndex_ = index;
}

auto AModel::GetIndices() const -> USpan<u32 const>
{
	return meshData_.indices;
}

void AModel::Load()
{
	meshData_ = SMeshLoader::Load(GetPath());
}

void AModel::CreateVertexBuffer(UDevice& device)
{
	VkDeviceSize const bufferSize{ sizeof(UVertex) * meshData_.vertices.size() };

	vertexBuffer_ = device.CreateAllocatedBuffer(
		  bufferSize
		, VK_BUFFER_USAGE_TRANSFER_DST_BIT
		| VK_BUFFER_USAGE_VERTEX_BUFFER_BIT
		| VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT
		, 0
	);

	device.StagingUpload(meshData_.vertices.data(), bufferSize, vertexBuffer_, 0, 0);
}
