#pragma once
#include <vulkan/vulkan.h>
#include <spirv-reflect/spirv_reflect.h>
#include <vector>
#include <map>
struct KtShaderLayout
{
	std::vector<VkShaderStageFlagBits>             ShaderStages;

	struct DescriptorSetLayout
	{
		struct DescriptorSetLayoutBinding
		{
			std::string           Name;
			uint32_t              Binding;
			VkDescriptorType      DescriptorType;
			uint32_t              DescriptorCount;
			VkShaderStageFlags    StageFlags;
			size_t                Size;
		};
		std::vector<DescriptorSetLayoutBinding>    DescriptorSetLayoutBindings;
	};
	std::map<size_t, DescriptorSetLayout>          DescriptorSetLayouts; // unordered?

	std::vector<VkVertexInputBindingDescription>   VertexInputBindingDescriptions;
	std::vector<VkVertexInputAttributeDescription> VertexInputAttributeDescriptions;

	std::vector<VkDescriptorType>                  DescriptorPoolTypes; // deducted
	std::vector<VkWriteDescriptorSet>              WriteDescriptorSets; // deducted
};

