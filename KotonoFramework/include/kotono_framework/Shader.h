#pragma once
#include <vulkan/vulkan.h>
#include <span>
#include <filesystem>
class KtShader
{
public:
	virtual ~KtShader() = default;

	virtual void Init() = 0;
	virtual void Cleanup();

	VkPipeline GetGraphicsPipeline() const;
	VkPipelineLayout GetPipelineLayout() const;

	void CmdBind(VkCommandBuffer commandBuffer) const;

protected:
	std::filesystem::path _vertPath;
	std::filesystem::path _fragPath;

	VkPipelineLayout _pipelineLayout;
	VkPipeline _graphicsPipeline;

	void SetVertPath(const std::filesystem::path& path);
	void SetFragPath(const std::filesystem::path& path);

	void CreateDescriptorSetLayout(VkDescriptorSetLayout& layout, const std::span<VkDescriptorSetLayoutBinding> layoutBindings);
	void CreateDescriptorPool(VkDescriptorPool& pool, const std::span<VkDescriptorPoolSize> poolSizes, const uint32_t setCount);
	void CreateShaderModule(VkShaderModule& shaderModule, const std::span<char> code);
	void CreateGraphicsPipeline(const std::span<VkVertexInputBindingDescription> bindingDescriptions, const std::span<VkVertexInputAttributeDescription> attributeDescriptions,	const VkPipelineRasterizationStateCreateInfo& rasterizer, const VkPipelineMultisampleStateCreateInfo& multisampling, const VkPipelineDepthStencilStateCreateInfo& depthStencil, const VkPipelineColorBlendAttachmentState& colorBlendAttachment, const std::span<VkDescriptorSetLayout> setLayouts);
};

