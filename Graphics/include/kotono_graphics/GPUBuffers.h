#pragma once
#include "frames_in_flight.h"
#include <glm/vec4.hpp>
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

	struct FrameData
	{
		UAllocatedBuffer clusterGridBuffer;
		UAllocatedBuffer lightIndexBuffer;
		UAllocatedBuffer lightCounterBuffer;
	};

public:
	void Init();
	void Cleanup() const;

	VkDeviceAddress GetClusterAABBAddress() const;
	VkDeviceAddress GetClusterGridAddress(const u32 frameIndex) const;
	VkDeviceAddress GetLightIndexAddress(const u32 frameIndex) const;
	VkDeviceAddress GetLightCounterAddress(const u32 frameIndex) const;

	VkBuffer GetLightCounterBuffer(const u32 frameIndex) const;

private:
	UAllocatedBuffer clusterAABBBuffer_;
	UFramesInFlightArray<FrameData> frameDatas_;
};
