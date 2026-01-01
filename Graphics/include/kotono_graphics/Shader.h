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
		VkDescriptorSetLayout                                        DescriptorSetLayout;
		KtFramesInFlightArray<VkDescriptorSet>                       DescriptorSets;
		std::string                                                  Name;
		KtFramesInFlightArray<KtAllocatedBuffer>                     Buffers;
		KtFramesInFlightArray<KtAllocatedBuffer>                     StagingBuffers;
		KtFramesInFlightArray<size_t>                                MemberCounts;
		size_t                                                       MemberSize;
		VkDescriptorType                                             DescriptorType;
		uint32_t                                                     Binding;
		uint32_t                                                     DescriptorCount;
		VkShaderStageFlags                                           ShaderStageFlags;
		VkDescriptorBindingFlags                                     BindingFlags;
		KtFramesInFlightArray<std::vector<VkDescriptorImageInfo>>    ImageInfos;
	};
	struct DescriptorSetLayoutData final
	{
	    VkDescriptorSetLayout                          DescriptorSetLayout;
		KtFramesInFlightArray<VkDescriptorSet>         DescriptorSets;
	    std::vector<DescriptorSetLayoutBindingData>    DescriptorSetLayoutBindingDatas;
	};

	KtShader(const UPath& path);

	void Init();
	void Cleanup();

	const UPath& Path() const;

	VkPipeline GetGraphicsPipeline() const;
	VkPipelineLayout GetPipelineLayout() const;

	void CmdBind(VkCommandBuffer commandBuffer) const;
	void CmdBindDescriptorSets(VkCommandBuffer commandBuffer, const uint32_t imageIndex) const;

	DescriptorSetLayoutBindingData* GetDescriptorSetLayoutBinding(const std::string& name);
	
	void UpdateDescriptorSetLayoutBindingBuffer(DescriptorSetLayoutBindingData& descriptorSetLayoutBindingData, const void* data, const uint32_t imageIndex);
	void UpdateDescriptorSetLayoutBindingBufferMemberCount(DescriptorSetLayoutBindingData& descriptorSetLayoutBindingData, const size_t memberCount, const uint32_t imageIndex);

	void UpdateDescriptorSetLayoutBindingImageSampler(DescriptorSetLayoutBindingData& descriptorSetLayoutBindingData, const std::vector<VkDescriptorImageInfo>& imageInfos, const uint32_t imageIndex);


protected:
	const UPath path_;

	KtShaderLayout shaderLayout_;

	VkPipelineLayout pipelineLayout_;
	VkPipeline graphicsPipeline_;

	VkDescriptorPool descriptorPool_;
	std::vector<DescriptorSetLayoutData> descriptorSetLayoutDatas_;
	std::unordered_map<std::string, DescriptorSetLayoutBindingData*> descriptorSetLayoutBindingDataRegistry_;

	void CreateShaderLayout();
	void PopulateShaderLayout(const std::span<uint8_t> spirvData, const VkShaderStageFlagBits shaderStage);

	void CreateDescriptorSetLayouts();
	void CreateDescriptorSetLayout(VkDescriptorSetLayout& layout, const std::span<VkDescriptorSetLayoutBinding> layoutBindings, const std::span<VkDescriptorBindingFlags> bindingFlags);

	void CreateDescriptorSetLayoutBindingBuffer(DescriptorSetLayoutBindingData& descriptorSetLayoutBindingData, const uint32_t imageIndex);
	void CreateDescriptorSetLayoutBindingImageSampler(DescriptorSetLayoutBindingData& descriptorSetLayoutBindingData, const uint32_t imageIndex);
	
	void CreateDescriptorSets();
	void CreateDescriptorSetLayoutBindings();

	void CreateDescriptorPools();
	void CreateDescriptorPool(const std::span<VkDescriptorPoolSize> poolSizes, const uint32_t setCount);
	
	void CreateGraphicsPipeline();
	void CreateShaderModule(VkShaderModule& shaderModule, const std::span<uint8_t> code);

	void DebugLogDescriptorSetLayoutData() const;

	bool GetIsBufferDescriptorType(const VkDescriptorType descriptorType) const;
	bool GetIsImageSamplerDescriptorType(const VkDescriptorType descriptorType) const;
	
	void UpdateDescriptorSetLayoutBindingBufferDescriptorSet(DescriptorSetLayoutBindingData& descriptorSetLayoutBindingData, const uint32_t imageIndex);
	void UpdateDescriptorSetLayoutBindingImageSamplerDescriptorSet(DescriptorSetLayoutBindingData& descriptorSetLayoutBindingData, const uint32_t imageIndex);
};
