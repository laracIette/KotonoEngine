#pragma once
#include <kotono_common/types.h>
#include <kotono_platform/AllocatedBuffer.h>
#include <kotono_platform/AllocatedImage.h>
#include <span>
#include <vector>
#include <vulkan/vulkan_core.h>
struct UDirectionalLight;
struct UFrameContextSceneView;
struct UPointLight;
class UPipelineResourceManager;
class ULightBuffers final
{
public:
	struct ShadowMapTarget
	{
		UAllocatedImage allocatedImage;
		u32 textureIndex;
	};

public:
	void Init(UPipelineResourceManager& pipelineResourceManager, VkDevice device, VmaAllocator allocator, VkFormat shadowMapFormat);
	void Cleanup(VkDevice device, VmaAllocator allocator) const;

	void UpdateBuffers(std::span<UDirectionalLight const> directionalLights, std::span<UPointLight const> pointLights) const;

	VkDeviceAddress GetDirectionalLightAddress() const;
	VkDeviceAddress GetPointLightAddress() const;

	u32 GetDirectionalLightShadowMapTargetIndex(u32 index) const;

	void CmdSetViewportAndScissor(VkCommandBuffer commandBuffer) const;
	void CmdBarrierShadowMapsNoneToWrite(VkCommandBuffer commandBuffer) const;
	void CmdBeginRenderingShadowMapTarget(VkCommandBuffer commandBuffer, u32 index) const;
	void CmdBarrierShadowMapsWriteToShaderRead(VkCommandBuffer commandBuffer) const;

private:
	void CreateBuffers(VkDevice device, VmaAllocator allocator);
	void CreateShadowMapResources(UPipelineResourceManager& pipelineResourceManager, VkDevice device, VmaAllocator allocator, VkFormat shadowMapFormat);

private:
	UAllocatedBuffer directionalLightBuffer_;
	UAllocatedBuffer pointLightBuffer_;
	std::vector<ShadowMapTarget> directionalLightShadowMapTargets_;
};
