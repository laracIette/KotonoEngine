#pragma once

#include "Containers/List.h"
#include "types.h"

#include <vulkan/vulkan_core.h>

class UDevice;

class UPipelineResourceManager final
{
public:
	struct ResourcePool
	{
		UList<u32> freeSlots;
		u32 nextSlot;
		u32 maxSlots;
	};

public:
	explicit UPipelineResourceManager(UDevice& device);

	void Init();
	void Cleanup() const;

	auto GetPipelineLayout() const -> VkPipelineLayout;
	auto GetDescriptorPool() const -> VkDescriptorPool;
	auto GetDescriptorSet() const -> VkDescriptorSet;

	auto RegisterTexture(VkImageView imageView) -> u32;
	auto RegisterTextureArray(VkImageView imageView) -> u32;
	auto RegisterSampler(VkSampler sampler) -> u32;
	auto RegisterShadowSampler(VkSampler sampler) -> u32;
	void UnregisterTexture(u32 slot);
	void UnregisterTextureArray(u32 slot);
	void UnregisterSampler(u32 slot);
	void UnregisterShadowSampler(u32 slot);

	void CmdBindDescriptorSet(VkCommandBuffer commandBuffer) const;

private:
	void CreateDescriptorSetLayout();
	void CreatePipelineLayout();
	void CreateDescriptorPool();
	void CreateDescriptorSet();

	static auto AllocateSlot(ResourcePool& resourcePool) -> u32;

	void WriteDescriptorSet(VkSampler sampler, VkImageView imageView, VkImageLayout imageLayout, u32 binding, u32 slot, VkDescriptorType descriptorType) const;

private:
	UDevice& device_;

	VkDescriptorSetLayout descriptorSetLayout_;
	VkDescriptorPool descriptorPool_;
	VkPipelineLayout pipelineLayout_;
	VkDescriptorSet descriptorSet_;

	ResourcePool texturePool_;
	ResourcePool textureArrayPool_;
	ResourcePool samplerPool_;
	ResourcePool shadowSamplerPool_;
};
