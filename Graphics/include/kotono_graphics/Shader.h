#pragma once
#include "frames_in_flight.h"
#include "ShaderLayout.h"
#include <array>
#include <kotono_common/Path.h>
#include <kotono_platform/AllocatedBuffer.h>
#include <span>
#include <unordered_map>
#include <vulkan/vulkan.h>
#include <kotono_io/serialize_base.h>
class KtShader final
{
public:	
	struct DescriptorSetLayoutBindingData final
	{														  
		VkDescriptorSetLayout										DescriptorSetLayout;
		KtFramesInFlightArray<VkDescriptorSet>						DescriptorSets;
		std::string													Name;
		KtFramesInFlightArray<KtAllocatedBuffer>					Buffers;
		KtFramesInFlightArray<KtAllocatedBuffer>					StagingBuffers;
		KtFramesInFlightArray<size>									MemberCounts;
		size														MemberSize;
		VkDescriptorType											DescriptorType;
		u32															Binding;
		u32															DescriptorCount;
		VkShaderStageFlags											ShaderStageFlags;
		VkDescriptorBindingFlags									BindingFlags;
		KtFramesInFlightArray<std::vector<VkDescriptorImageInfo>>	ImageInfos;
	};
	struct DescriptorSetLayoutData final
	{
	    VkDescriptorSetLayout                          DescriptorSetLayout;
		KtFramesInFlightArray<VkDescriptorSet>         DescriptorSets;
	    std::vector<DescriptorSetLayoutBindingData>    DescriptorSetLayoutBindingDatas;
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
	void CreateShaderLayout();
	void PopulateShaderLayout(const std::span<u8> spirvData, const VkShaderStageFlagBits shaderStage);

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
	
	void UpdateDescriptorSetLayoutBindingBufferDescriptorSet(DescriptorSetLayoutBindingData& descriptorSetLayoutBindingData, const u32 imageIndex);
	void UpdateDescriptorSetLayoutBindingImageSamplerDescriptorSet(DescriptorSetLayoutBindingData& descriptorSetLayoutBindingData, const u32 imageIndex);

private:
	const UPath path_;

	KtShaderLayout shaderLayout_;

	VkPipelineLayout pipelineLayout_;
	VkPipeline graphicsPipeline_;

	VkDescriptorPool descriptorPool_;
	std::vector<DescriptorSetLayoutData> descriptorSetLayoutDatas_;
	std::unordered_map<std::string, DescriptorSetLayoutBindingData*> descriptorSetLayoutBindingDataRegistry_;
};

template <>
struct USerialize<KtShader>
{
	void operator()(nlohmann::json& json, const KtShader* v) const;
};

template <>
struct UDeserialize<KtShader>
{
	void operator()(const nlohmann::json& json, KtShader*& v) const;
};
