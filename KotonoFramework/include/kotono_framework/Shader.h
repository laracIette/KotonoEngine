#pragma once
#include <vulkan/vulkan.h>
#include <span>
#include <filesystem>
#include <array>
#include "frames_in_flight.h"
#include "AllocatedBuffer.h"
class KtShader
{
public:
	virtual ~KtShader() = default;

	virtual void Init();
	virtual void Cleanup();

	VkPipeline GetGraphicsPipeline() const;
	VkPipelineLayout GetPipelineLayout() const;

	void CmdBind(VkCommandBuffer commandBuffer) const;
	void CmdBindDescriptorSets(VkCommandBuffer commandBuffer, const uint32_t imageIndex) const;

protected:
	std::filesystem::path _vertPath;
	std::filesystem::path _fragPath;

	VkDeviceSize _uniformDataSize;
	VkDeviceSize _objectDataSize;

	VkPipelineLayout _pipelineLayout;
	VkPipeline _graphicsPipeline;

	VkDescriptorPool _descriptorPool;
	VkDescriptorSetLayout _uniformDescriptorSetLayout;
	VkDescriptorSetLayout _objectDescriptorSetLayout;
	std::array<VkDescriptorSet, KT_FRAMES_IN_FLIGHT> _uniformDescriptorSets;
	std::array<VkDescriptorSet, KT_FRAMES_IN_FLIGHT> _objectDescriptorSets;

	std::array<KtAllocatedBuffer, KT_FRAMES_IN_FLIGHT> _uniformBuffers;
	std::array<KtAllocatedBuffer, KT_FRAMES_IN_FLIGHT> _objectBuffers;
	std::array<KtAllocatedBuffer, KT_FRAMES_IN_FLIGHT> _stagingUniformBuffers;
	std::array<KtAllocatedBuffer, KT_FRAMES_IN_FLIGHT> _stagingObjectBuffers;
	std::array<VkDeviceSize, KT_FRAMES_IN_FLIGHT> _objectCounts;

	void SetVertPath(const std::filesystem::path& path);
	void SetFragPath(const std::filesystem::path& path);

	void SetUniformDataSize(const VkDeviceSize size);
	void SetObjectDataSize(const VkDeviceSize size);

	void CreateDescriptorSetLayout(VkDescriptorSetLayout& layout, const std::span<VkDescriptorSetLayoutBinding> layoutBindings);
	void CreateDescriptorSets();	
	void CreateDescriptorPool(const std::span<VkDescriptorPoolSize> poolSizes, const uint32_t setCount);
	void CreateShaderModule(VkShaderModule& shaderModule, const std::span<char> code);
	void CreateGraphicsPipeline(const std::span<VkVertexInputBindingDescription> bindingDescriptions, const std::span<VkVertexInputAttributeDescription> attributeDescriptions,	const VkPipelineRasterizationStateCreateInfo& rasterizer, const VkPipelineMultisampleStateCreateInfo& multisampling, const VkPipelineDepthStencilStateCreateInfo& depthStencil, const VkPipelineColorBlendAttachmentState& colorBlendAttachment, const std::span<VkDescriptorSetLayout> setLayouts);


	void CreateUniformBuffers();
	void CreateUniformBuffer(const uint32_t imageIndex);
	void CreateObjectBuffers();
	void CreateObjectBuffer(const uint32_t imageIndex);
	void SetObjectCount(const VkDeviceSize objectCount, const uint32_t imageIndex);
	// Recreates the object buffer when the object count changes
	const VkDeviceSize GetObjectBufferCount(const uint32_t imageIndex) const;
	
	virtual void CreateDescriptorSetLayouts() = 0;
	virtual void CreateDescriptorPools() = 0;
	virtual void UpdateDescriptorSet(const uint32_t imageIndex) = 0;
	virtual void CreateGraphicsPipelines() = 0;
};

