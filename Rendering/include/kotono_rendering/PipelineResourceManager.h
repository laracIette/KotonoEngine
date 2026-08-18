#pragma once
#include <kotono_common/types.h>
#include <vector>
#include <vulkan/vulkan_core.h>
class GPipelineResourceManager final
{
public:
	struct ResourcePool
	{
		std::vector<u32> freeSlots;
		u32 nextSlot;
		u32 maxSlots;
	};

public:
	void Init();
	void Cleanup() const;

	VkPipelineLayout GetPipelineLayout() const;
	VkDescriptorPool GetDescriptorPool() const;
	VkDescriptorSet GetDescriptorSet() const;

	u32 RegisterTexture(VkImageView imageView);
	u32 RegisterTextureArray(VkImageView imageView);
	u32 RegisterSampler(VkSampler sampler);
	u32 RegisterShadowSampler(VkSampler sampler);
	void UnregisterTexture(const u32 slot);
	void UnregisterTextureArray(const u32 slot);
	void UnregisterSampler(const u32 slot);
	void UnregisterShadowSampler(const u32 slot);

	void CmdBindDescriptorSet(VkCommandBuffer commandBuffer) const;

private:
	void CreateDescriptorSetLayout();
	void CreatePipelineLayout();
	void CreateDescriptorPool();
	void CreateDescriptorSet();

	u32 AllocateSlot(ResourcePool& resourcePool) const;

	void WriteDescriptorSet(VkSampler sampler, VkImageView imageView, const VkImageLayout imageLayout, const u32 binding, const u32 slot, const VkDescriptorType descriptorType) const;

private:
	VkDescriptorSetLayout descriptorSetLayout_;
	VkDescriptorPool descriptorPool_;
	VkPipelineLayout pipelineLayout_;
	VkDescriptorSet descriptorSet_;

	ResourcePool texturePool_;
	ResourcePool textureArrayPool_;
	ResourcePool samplerPool_;
	ResourcePool shadowSamplerPool_;
};

inline GPipelineResourceManager PipelineResourceManager;
