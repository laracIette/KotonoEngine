#pragma once

#include "Asset/Asset.h"

#include "AllocatedBuffer.h"
#include "Containers/Span.h"
#include "MeshLoader/MeshData.h"
#include "types.h"

class UDevice;
class UIndexBuffer;

class AModel final : public AAsset
{
public:
	explicit AModel(UPath const& path);

	void Init(UDevice& device);
	void Cleanup(UDevice& device) const;

	auto GetVertexBufferAddress() const -> VkDeviceAddress;
	auto GetIndexCount() const -> u32;
	auto GetFirstIndex() const -> u32;

	void SetFirstIndex(u32 index);

	auto GetIndices() const -> USpan<u32 const>;
	
private:
	void Load();
	void CreateVertexBuffer(UDevice& device);

private:
	UMeshData meshData_;
	u32 firstIndex_;

	UAllocatedBuffer vertexBuffer_;
};
