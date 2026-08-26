#pragma once
#include <glm/ext/vector_float4.hpp>
#include <kotono_common/types.h>
#include <kotono_platform/AllocatedBuffer.h>
#include <vulkan/vulkan_core.h>
class UGPUBuffers final
{
public:
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
	void Init(VkDevice device, VmaAllocator allocator);
	void Cleanup(VmaAllocator allocator) const;

	VkDeviceAddress GetClusterAABBAddress() const;
	VkDeviceAddress GetClusterGridAddress() const;
	VkDeviceAddress GetLightIndexAddress() const;
	VkDeviceAddress GetLightCounterAddress() const;

	VkBuffer GetLightCounterBuffer() const;

private:
	UAllocatedBuffer clusterAABBBuffer_; 
	UAllocatedBuffer clusterGridBuffer_;
	UAllocatedBuffer lightIndexBuffer_;
	UAllocatedBuffer lightCounterBuffer_;
};
