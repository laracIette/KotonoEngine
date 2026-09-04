#pragma once
#include <glm/ext/vector_float4.hpp>
#include <kotono_common/types.h>
#include <kotono_platform/AllocatedBuffer.h>
#include <vulkan/vulkan_core.h>
class UDevice;
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
	explicit UGPUBuffers(UDevice& device);

	void Init();
	void Cleanup() const;

	VkDeviceAddress GetClusterAABBAddress() const;
	VkDeviceAddress GetClusterGridAddress() const;
	VkDeviceAddress GetLightIndexAddress() const;
	VkDeviceAddress GetLightCounterAddress() const;

	VkBuffer GetLightCounterBuffer() const;

private:
	UDevice& device_;

	UAllocatedBuffer clusterAABBBuffer_; 
	UAllocatedBuffer clusterGridBuffer_;
	UAllocatedBuffer lightIndexBuffer_;
	UAllocatedBuffer lightCounterBuffer_;
};
