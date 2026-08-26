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
	void Init(UPipelineResourceManager& pipelineResourceManager);
	void Cleanup() const;

	void UpdateBuffers(UFrameContextSceneView const& sceneView, std::span<UDirectionalLight const> directionalLights, std::span<UPointLight const> pointLights) const;

	VkDeviceAddress GetDirectionalLightAddress() const;
	VkDeviceAddress GetPointLightAddress() const;

	u32 GetDirectionalLightShadowMapTargetIndex(u32 index) const;

	void CmdSetViewportAndScissor(VkCommandBuffer commandBuffer) const;
	void CmdBarrierShadowMapsNoneToWrite(VkCommandBuffer commandBuffer) const;
	void CmdBeginRenderingShadowMapTarget(VkCommandBuffer commandBuffer, u32 index) const;
	void CmdBarrierShadowMapsWriteToShaderRead(VkCommandBuffer commandBuffer) const;

private:
	void CreateBuffers();
	void CreateShadowMapResources(UPipelineResourceManager& pipelineResourceManager);

private:
	UAllocatedBuffer directionalLightBuffer_;
	UAllocatedBuffer pointLightBuffer_;
	std::vector<ShadowMapTarget> directionalLightShadowMapTargets_;
};
