#pragma once
#include <vulkan/vulkan.h>
#include <span>
#include <filesystem>
#include <array>
#include "frames_in_flight.h"
#include "AllocatedBuffer.h"
#include "ShaderLayout.h"
#include "ImageTexture.h"
class KtShader final
{
public:	
	struct DescriptorSetLayoutBindingData
	{														  
		std::string                                           Name;
		std::array<KtAllocatedBuffer, KT_FRAMES_IN_FLIGHT>    Buffers;
		std::array<KtAllocatedBuffer, KT_FRAMES_IN_FLIGHT>    StagingBuffers;
		std::array<size_t, KT_FRAMES_IN_FLIGHT>               MemberCounts;
		size_t                                                MemberSize;
		VkDescriptorType                                      DescriptorType;
		uint32_t                                              Binding;
		uint32_t                                              DescriptorCount;
		VkShaderStageFlags                                    ShaderStageFlags;
		KtImageTexture*                                       ImageTexture;

	};
	struct DescriptorSetLayoutData
	{
	    VkDescriptorSetLayout                               DescriptorSetLayout;
	    std::array<VkDescriptorSet, KT_FRAMES_IN_FLIGHT>    DescriptorSets;
	    std::vector<DescriptorSetLayoutBindingData>         DescriptorSetLayoutBindingDatas;
	};

	KtShader(const std::filesystem::path& path);

	void Init();
	void Cleanup();

	const std::filesystem::path& GetPath() const;

	const std::string& GetName() const;
	void SetName(const std::string& name);

	VkPipeline GetGraphicsPipeline() const;
	VkPipelineLayout GetPipelineLayout() const;

	void CmdBind(VkCommandBuffer commandBuffer) const;
	void CmdBindDescriptorSets(VkCommandBuffer commandBuffer, const uint32_t imageIndex) const;

	DescriptorSetLayoutBindingData* GetDescriptorSetLayoutBinding(const std::string_view name);
	
	void UpdateDescriptorSetLayoutBindingBuffer(DescriptorSetLayoutBindingData& descriptorSetLayoutBindingData, void* data, const uint32_t imageIndex);
	void UpdateDescriptorSetLayoutBindingBufferMemberCount(DescriptorSetLayoutBindingData& descriptorSetLayoutBindingData, const size_t memberCount, const uint32_t imageIndex);

	void UpdateDescriptorSetLayoutBindingImage(DescriptorSetLayoutBindingData& descriptorSetLayoutBindingData, KtImageTexture* imageTexture);

protected:
	std::string _name;

	const std::filesystem::path _path;

	KtShaderLayout _shaderLayout;

	VkDeviceSize _uniformDataSize;
	VkDeviceSize _objectDataSize;

	VkPipelineLayout _pipelineLayout;
	VkPipeline _graphicsPipeline;

	VkDescriptorPool _descriptorPool;
	std::vector<DescriptorSetLayoutData> _descriptorSetLayoutDatas;
	std::array<VkDeviceSize, KT_FRAMES_IN_FLIGHT> _objectCounts;

	void CreateShaderLayout();
	void PopulateShaderLayout(const std::span<uint8_t> spirvData, const VkShaderStageFlagBits shaderStage);

	void CreateDescriptorSetLayouts();
	void CreateDescriptorSetLayout(VkDescriptorSetLayout& layout, const std::span<VkDescriptorSetLayoutBinding> layoutBindings);

	void CreateDescriptorSetLayoutBindingBuffers();
	void CreateDescriptorSetLayoutBindingBuffers(const uint32_t imageIndex);
	void CreateDescriptorSetLayoutBindingBuffer(DescriptorSetLayoutBindingData& descriptorSetLayoutBindingData);
	void CreateDescriptorSetLayoutBindingBuffer(DescriptorSetLayoutBindingData& descriptorSetLayoutBindingData, const uint32_t imageIndex);

	void CreateDescriptorSetLayoutBindingImages();
	void CreateDescriptorSetLayoutBindingImage(DescriptorSetLayoutBindingData& descriptorSetLayoutBindingData);
	
	void CreateDescriptorSets();
	void UpdateDescriptorSets(const uint32_t imageIndex);

	void CreateDescriptorPools();
	void CreateDescriptorPool(const std::span<VkDescriptorPoolSize> poolSizes, const uint32_t setCount);
	
	void CreateGraphicsPipeline();
	void CreateShaderModule(VkShaderModule& shaderModule, const std::span<uint8_t> code);

	void DebugLogDescriptorSetLayoutData() const;
};

