#pragma once
#include "frames_in_flight.h"
#include "Lights.h"
#include <kotono_common/types.h>
#include <kotono_platform/AllocatedImage.h>
#include <vector>
#include <vma/vk_mem_alloc.h>
#include <vulkan/vulkan_core.h>
class GLightBuffers final
{
public:
	struct ShadowMapTarget
	{
		UAllocatedImage allocatedImage;
		u32 textureIndex;
	};

	struct LightBuffer
	{
		VkBuffer buffer;
		VmaAllocation allocation;
		void* mapped;
		VkDeviceAddress bda;
	};
	
	struct FrameData
	{
		LightBuffer directionalLightBuffer;
		std::vector<ShadowMapTarget> directionalLightShadowMapTargets;

		LightBuffer pointLightBuffer;
	};

	struct DirectionalLightData
	{
		glm::vec3 direction;
		glm::vec3 color;
		f32 intensity;
		bool castShadow;
		glm::mat4 lightViewProj;
	};

public:
	void Init();
	void Cleanup() const;

	void RegisterDirectionalLight(const DirectionalLightData& directionalLight);
	void RegisterPointLight(const UPointLight& pointLight);

	void UpdateBuffers(const u32 frameIndex);

	VkDeviceAddress GetDirectionalLightAddress(const u32 frameIndex) const;
	VkDeviceAddress GetPointLightAddress(const u32 frameIndex) const;

	u32 GetDirectionalLightCount() const;
	u32 GetPointLightCount() const;

	void CmdBarrierShadowMapsNoneToWrite(VkCommandBuffer commandBuffer, const u32 frameIndex) const;
	void CmdBeginRenderingShadowMapTarget(VkCommandBuffer commandBuffer, const u32 index, const u32 frameIndex) const;
	void CmdBarrierShadowMapsWriteToShaderRead(VkCommandBuffer commandBuffer, const u32 frameIndex) const;

private:
	void CreateBuffers();
	void CreateBuffer(LightBuffer& lightBuffer, const VkDeviceSize size);
	void CreateShadowMapResources();

private:
	UFramesInFlightArray<FrameData> frameDatas_;

	std::vector<UDirectionalLight> directionalLights_;
	std::vector<UPointLight> pointLights_;
};

inline GLightBuffers LightBuffers;
