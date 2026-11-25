#pragma once

#include "Containers/List.h"
#include "AllocatedBuffer.h"
#include "AllocatedImage.h"
#include "types.h"

#include <vulkan/vulkan_core.h>

struct UDirectionalLight;
struct UFrameContextSceneView;
struct UPointLight;
class UDevice;
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
	explicit ULightBuffers(UDevice& device, UPipelineResourceManager& pipelineResourceManager);

	void Init();
	void Cleanup() const;

	void UpdateBuffers(USpan<UDirectionalLight const> directionalLights, USpan<UPointLight const> pointLights) const;

	auto GetDirectionalLightAddress() const -> VkDeviceAddress;
	auto GetPointLightAddress() const -> VkDeviceAddress;

	auto GetDirectionalLightShadowMapTargetIndex(u32 index) const -> u32;

	void CmdSetViewportAndScissor(VkCommandBuffer commandBuffer) const;
	void CmdBarrierShadowMapsNoneToWrite(VkCommandBuffer commandBuffer) const;
	void CmdBeginRenderingShadowMapTarget(VkCommandBuffer commandBuffer, u32 index) const;
	void CmdBarrierShadowMapsWriteToShaderRead(VkCommandBuffer commandBuffer) const;

private:
	void CreateBuffers();
	void CreateShadowMapResources();

private:
	UDevice& device_;
	UPipelineResourceManager& pipelineResourceManager_;

	UAllocatedBuffer directionalLightBuffer_;
	UAllocatedBuffer pointLightBuffer_;
	UList<ShadowMapTarget> directionalLightShadowMapTargets_;
};
