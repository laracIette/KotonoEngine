#pragma once
#include "frames_in_flight.h"
#include "ShaderLayout.h"
#include <array>
#include <kotono_common/Path.h>
#include <kotono_platform/AllocatedBuffer.h>
#include <span>
#include <unordered_map>
#include <vulkan/vulkan.h>
class KtShader final
{
public:	
	struct DescriptorSetLayoutBindingData final
	{														  
		VkDescriptorSetLayout										descriptorSetLayout;
		KtFramesInFlightArray<VkDescriptorSet>						descriptorSets;
		std::string													name;
		KtFramesInFlightArray<KtAllocatedBuffer>					buffers;
		KtFramesInFlightArray<KtAllocatedBuffer>					stagingBuffers;
		KtFramesInFlightArray<size>									memberCounts;
		size														memberSize;
		VkDescriptorType											descriptorType;
		u32															binding;
		u32															descriptorCount;
		VkShaderStageFlags											shaderStageFlags;
		KtFramesInFlightArray<std::vector<VkDescriptorImageInfo>>	imageInfos;
		bool														isBindless;
	};
	struct DescriptorSetLayoutData final
	{
	    VkDescriptorSetLayout                          descriptorSetLayout;
		KtFramesInFlightArray<VkDescriptorSet>         descriptorSets;
	    std::vector<DescriptorSetLayoutBindingData>    descriptorSetLayoutBindingDatas;
	};

	KtShader(const UPath& path);
	~KtShader();

	const UPath& Path() const;

	VkPipeline GetGraphicsPipeline() const;
	VkPipelineLayout GetPipelineLayout() const;

	void CmdBind(VkCommandBuffer commandBuffer) const;
	void CmdBindDescriptorSets(VkCommandBuffer commandBuffer, const u32 imageIndex) const;

	DescriptorSetLayoutBindingData* GetDescriptorSetLayoutBinding(const std::string& name);
	
	void UpdateDescriptorSetLayoutBindingBuffer(DescriptorSetLayoutBindingData& descriptorSetLayoutBindingData, const void* data, const u32 imageIndex);
	void UpdateDescriptorSetLayoutBindingBufferMemberCount(DescriptorSetLayoutBindingData& descriptorSetLayoutBindingData, const size memberCount, const u32 imageIndex);

	void UpdateDescriptorSetLayoutBindingImageSampler(DescriptorSetLayoutBindingData& descriptorSetLayoutBindingData, const std::vector<VkDescriptorImageInfo>& imageInfos, const u32 imageIndex);

private:
	void CreateDescriptorSetLayouts();
	void CreateDescriptorSetLayout(VkDescriptorSetLayout& layout, const std::span<VkDescriptorSetLayoutBinding> layoutBindings, const std::span<VkDescriptorBindingFlags> bindingFlags);

	void CreateDescriptorSetLayoutBindingBuffer(DescriptorSetLayoutBindingData& descriptorSetLayoutBindingData, const u32 imageIndex);
	void CreateDescriptorSetLayoutBindingImageSampler(DescriptorSetLayoutBindingData& descriptorSetLayoutBindingData, const u32 imageIndex);
	
	void CreateDescriptorSets();
	void CreateDescriptorSetLayoutBindings();

	void CreateDescriptorPools();
	void CreateDescriptorPool(const std::span<VkDescriptorPoolSize> poolSizes, const u32 setCount);
	
	void CreateGraphicsPipeline();
	void CreateShaderModule(VkShaderModule& shaderModule, const std::span<u8> code);

	void DebugLogDescriptorSetLayoutData() const;

	bool GetIsBufferDescriptorType(const VkDescriptorType descriptorType) const;
	bool GetIsImageSamplerDescriptorType(const VkDescriptorType descriptorType) const;
	VkBufferUsageFlagBits GetBufferUsageFlagBits(const VkDescriptorType descriptorType) const;
	
	void UpdateDescriptorSetLayoutBindingBufferDescriptorSet(DescriptorSetLayoutBindingData& descriptorSetLayoutBindingData, const u32 imageIndex);
	void UpdateDescriptorSetLayoutBindingImageSamplerDescriptorSet(DescriptorSetLayoutBindingData& descriptorSetLayoutBindingData, const u32 imageIndex);

	void CreateShaderLayout();

private:
	const UPath path_;

	KtShaderLayout shaderLayout_;

	VkPipelineLayout pipelineLayout_;
	VkPipeline graphicsPipeline_;

	VkDescriptorPool descriptorPool_;
	std::vector<DescriptorSetLayoutData> descriptorSetLayoutDatas_;
	std::unordered_map<std::string, DescriptorSetLayoutBindingData*> descriptorSetLayoutBindingDataRegistry_;
};
