#pragma once
#include "frames_in_flight.h"
#include <glm/mat4x4.hpp>
#include <kotono_common/types.h>
#include <vector>
#include <vma/vk_mem_alloc.h>
#include <vulkan/vulkan_core.h>

struct UPushConstants
{
	VkDeviceAddress drawDataAddress;
	VkDeviceAddress materialAddress;
	VkDeviceAddress transformAddress;
	VkDeviceAddress vertexBufferAddress;
	u32 drawIndex;
	u32 flags;
};

class SPipelineResourceManager final
{
public:
	struct FrameUBO
	{
		glm::mat4 view;
		glm::mat4 proj;
		glm::mat4 viewProj;
		glm::vec4 viewPos;
		f32 time;
	};

	struct FrameData
	{
		VkBuffer ubo;
		VmaAllocation uboAllocation;
		FrameUBO* uboMapped;
	};

public:
	void Init();
	void Cleanup() const;

	VkPipelineLayout GetPipelineLayout() const;
	VkDescriptorPool GetDescriptorPool() const;
	VkDescriptorSet GetGlobalDescriptorSet() const;

	void SetFrameUBO(const FrameUBO& frameUBO);
	void UpdateMappedFrameUBO(const u32 frameIndex) const;

	u32 RegisterTexture(VkImageView imageView, VkSampler sampler);
	void UnregisterTexture(const u32 slot);

	void CmdBindGlobalDescriptorSets(VkCommandBuffer commandBuffer) const;
	void CmdPushUniformDescriptorSet(VkCommandBuffer commandBuffer, const u32 frameIndex) const;

private:
	void CreateGlobalDescriptorSetLayout();
	void CreateUniformDescriptorSetLayout();
	void CreatePipelineLayout();
	void CreateDescriptorPool();
	void CreateGlobalDescriptorSet();
	void CreateFrameDataBuffers();

	u32 AllocateSlot();

private:
	KtFramesInFlightArray<FrameData> frameDatas_;
	FrameUBO frameUBO_;

	VkDescriptorSetLayout globalDescriptorSetLayout_;
	VkDescriptorSetLayout uniformDescriptorSetLayout_;
	VkDescriptorPool descriptorPool_;
	VkPipelineLayout pipelineLayout_;
	VkDescriptorSet globalDescriptorSet_;

	std::vector<u32> freeSampledSlots_;
	u32 nextSampledSlot_;
};

inline SPipelineResourceManager PipelineResourceManager;
