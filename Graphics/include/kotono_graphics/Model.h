#pragma once
#include "frames_in_flight.h"
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <kotono_common/Path.h>
#include <kotono_common/types.h>
#include <kotono_platform/AllocatedBuffer.h>
struct UVertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 uv;
	glm::vec4 tangent;

	bool operator==(const UVertex& other) const noexcept;
};
class UModel final
{
public:
	UModel(const UPath& path);
	~UModel();

	const UPath& Path() const;

	VkDeviceAddress GetVertexBufferAddress() const;
	VkBuffer GetIndexBuffer() const;
	u32 GetIndexCount() const;
	
private:
	void Load();
	void CreateVertexBuffer();
	void CreateIndexBuffer();
	void DestroyStagingVertexBuffer() const;
	void DestroyStagingIndexBuffer() const;

private:
	const UPath path_;

	std::vector<UVertex> vertices_;
	std::vector<u32> indices_;
	KtAllocatedBuffer vertexBuffer_;
	KtAllocatedBuffer indexBuffer_;
	KtAllocatedBuffer stagingVertexBuffer_;
	KtAllocatedBuffer stagingIndexBuffer_;
};

template<>
struct std::hash<UVertex>
{
	::size operator()(const UVertex& v) const noexcept;
};
