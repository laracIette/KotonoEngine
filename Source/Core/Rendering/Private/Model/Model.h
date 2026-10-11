#pragma once
#include <Asset/Asset.h>

#include <AllocatedBuffer.h>
#include <MeshLoader/MeshData.h>
#include <types.h>
#include <span>

class UDevice;
class UIndexBuffer;

class AModel final : public AAsset
{
public:
	explicit AModel(UPath const& path);

	void Init(UDevice& device);
	void Cleanup(UDevice& device) const;

	VkDeviceAddress GetVertexBufferAddress() const;
	u32 GetIndexCount() const;
	u32 GetFirstIndex() const;

	void SetFirstIndex(u32 index);

	std::span<u32 const> GetIndices() const;
	
private:
	void Load();
	void CreateVertexBuffer(UDevice& device);

private:
	UMeshData meshData_;
	u32 firstIndex_;

	UAllocatedBuffer vertexBuffer_;
};
