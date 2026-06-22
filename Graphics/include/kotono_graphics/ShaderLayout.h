#pragma once
#include <kotono_common/types.h>
#include <map>
#include <span>
#include <string>
#include <vector>
#include <vulkan/vulkan_core.h>
struct SpvReflectShaderModule;
struct KtShaderLayout final
{
	void Populate(const std::span<u8> spirvData, const VkShaderStageFlagBits shaderStage);
	void PopulateDescriptorSets(const SpvReflectShaderModule& module, const VkShaderStageFlagBits shaderStage);
	void PopulatePushConstants(const SpvReflectShaderModule& module);
	void PopulateVertexInputs(const SpvReflectShaderModule& module);

	struct DescriptorSetLayout final
	{
		struct Binding final
		{
			std::string				name;
			u32						binding;
			VkDescriptorType		descriptorType;
			u32						descriptorCount;
			VkShaderStageFlags		shaderStageFlags;
			size					size;
			bool					isBindless;
		};

		std::vector<Binding>    bindings;
	};

	std::vector<VkShaderStageFlagBits>				shaderStages;
													  
	std::map<size, DescriptorSetLayout>				descriptorSetLayouts;
	 												  
	std::vector<VkVertexInputBindingDescription>	vertexInputBindingDescriptions;
	std::vector<VkVertexInputAttributeDescription>	vertexInputAttributeDescriptions;
};

