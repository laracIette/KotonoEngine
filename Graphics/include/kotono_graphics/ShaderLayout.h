#pragma once
#include <vulkan/vulkan_core.h>
#include <vector>
#include <string>
#include <map>
struct KtShaderLayout final
{
	struct DescriptorSetLayout final
	{
		struct Binding final
		{
			std::string           Name; // todo: maybe const char*
			u32              Binding;
			VkDescriptorType      DescriptorType;
			u32              DescriptorCount;
			VkShaderStageFlags    ShaderStageFlags;
			size                Size;
		};

		std::vector<Binding>    Bindings;
	};

	std::vector<VkShaderStageFlagBits>                ShaderStages;
													  
	std::map<size, DescriptorSetLayout>             DescriptorSetLayouts;
	 												  
	std::vector<VkVertexInputBindingDescription>      VertexInputBindingDescriptions;
	std::vector<VkVertexInputAttributeDescription>    VertexInputAttributeDescriptions;
													  
	std::vector<VkDescriptorType>                     DescriptorPoolTypes; // deducted
	std::vector<VkWriteDescriptorSet>                 WriteDescriptorSets; // deducted
};

