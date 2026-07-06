#pragma once
#include <glm/vec4.hpp>
#include <kotono_common/types.h>
#include <vma/vk_mem_alloc.h>
#include <vulkan/vulkan_core.h>
class GGPUBuffers final
{
public:
	struct AllocatedBuffer
	{
		VkBuffer buffer;
		VmaAllocation allocation;
		VkDeviceAddress bda;
	};

	struct ClusterAABB
	{
		glm::vec4 minPoint;
		glm::vec4 maxPoint;
	};

	struct ClusterGrid
	{
		u32 offset;
		u32 count;
	};

public:
	void Init();
	void Cleanup() const;

	VkDeviceAddress GetClusterAABBAddress() const;
	VkDeviceAddress GetClusterGridAddress() const;
	VkDeviceAddress GetLightIndexAddress() const;
	VkDeviceAddress GetLightCounterAddress() const;

	VkBuffer GetLightCounterBuffer() const;

private:
	void CreateBuffer(AllocatedBuffer& allocatedBuffer, const VkDeviceSize size, const VkBufferUsageFlags usage);

private:
	AllocatedBuffer clusterAABBBuffer_;
	AllocatedBuffer clusterGridBuffer_;
	AllocatedBuffer lightIndexBuffer_;
	AllocatedBuffer lightCounterBuffer_;
};

inline GGPUBuffers GPUBuffers;
