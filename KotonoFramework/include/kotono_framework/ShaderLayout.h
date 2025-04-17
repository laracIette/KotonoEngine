#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <string>
#include <map>
struct KtShaderLayout
{
	std::vector<VkShaderStageFlagBits>              ShaderStages;

	struct DescriptorSetLayout
	{
		struct DescriptorSetLayoutBinding
		{
			std::string           Name; // todo: maybe const char*
			uint32_t              Binding;
			VkDescriptorType      DescriptorType;
			uint32_t              DescriptorCount;
			VkShaderStageFlags    StageFlags;
			size_t                Size;
		};
		std::vector<DescriptorSetLayoutBinding>     DescriptorSetLayoutBindings;
	};
	std::map<size_t, DescriptorSetLayout>           DescriptorSetLayouts; // for some reason map unordered_map crashes
	 
	std::vector<VkVertexInputBindingDescription>    VertexInputBindingDescriptions;
	std::vector<VkVertexInputAttributeDescription>  VertexInputAttributeDescriptions;

	std::vector<VkDescriptorType>                   DescriptorPoolTypes; // deducted
	std::vector<VkWriteDescriptorSet>               WriteDescriptorSets; // deducted
};

