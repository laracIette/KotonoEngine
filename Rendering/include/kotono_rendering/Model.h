#pragma once
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_float4.hpp>
#include <kotono_common/Asset.h>
#include <kotono_common/Path.h>
#include <kotono_common/types.h>
#include <kotono_platform/AllocatedBuffer.h>
#include <span>
class UIndexBuffer;
struct UVertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 uv;
	glm::vec4 tangent;

	bool operator==(UVertex const& other) const noexcept;
};
class AModel final : public AAsset
{
public:
	AModel(UPath const& path);
	~AModel() override;

	VkDeviceAddress GetVertexBufferAddress() const;
	u32 GetIndexCount() const;
	u32 GetFirstIndex() const;

	void SetFirstIndex(u32 index);

	std::span<u32 const> GetIndices() const;
	
private:
	void Load();
	void CreateVertexBuffer();
	void DestroyStagingVertexBuffer() const;

private:
	std::vector<UVertex> vertices_;

	std::vector<u32> indices_;
	u32 firstIndex_;

	UAllocatedBuffer vertexBuffer_;
	UAllocatedBuffer stagingVertexBuffer_;
};

template<>
struct std::hash<UVertex>
{
	::size operator()(UVertex const& v) const noexcept;
};
