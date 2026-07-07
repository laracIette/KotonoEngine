#pragma once
#include <kotono_common/types.h>
#include <vector>
#include <vulkan/vulkan_core.h>

struct UPushConstants
{
	VkDeviceAddress frameContextBufferAddress;
	VkDeviceAddress vertexBufferAddress;
	u32 drawIndex;
};

class GPipelineResourceManager final
{
public:
	void Init();
	void Cleanup() const;

	VkPipelineLayout GetPipelineLayout() const;
	VkDescriptorPool GetDescriptorPool() const;
	VkDescriptorSet GetDescriptorSet() const;

	u32 RegisterTexture(VkImageView imageView);
	u32 RegisterSampler(VkSampler sampler);
	u32 RegisterSamplerShadow(VkSampler sampler);
	void UnregisterTexture(const u32 slot);
	void UnregisterSampler(const u32 slot);
	void UnregisterSamplerShadow(const u32 slot);

	void CmdBindDescriptorSet(VkCommandBuffer commandBuffer) const;

private:
	void CreateDescriptorSetLayout();
	void CreatePipelineLayout();
	void CreateDescriptorPool();
	void CreateDescriptorSet();

	u32 AllocateTextureSlot();
	u32 AllocateSamplerSlot();
	u32 AllocateSamplerShadowSlot();

private:
	VkDescriptorSetLayout descriptorSetLayout_;
	VkDescriptorPool descriptorPool_;
	VkPipelineLayout pipelineLayout_;
	VkDescriptorSet descriptorSet_;

	std::vector<u32> freeTextureSlots_;
	u32 nextTextureSlot_;
	std::vector<u32> freeSamplerSlots_;
	u32 nextSamplerSlot_;
	std::vector<u32> freeSamplerShadowSlots_;
	u32 nextSamplerShadowSlot_;
};

inline GPipelineResourceManager PipelineResourceManager;
