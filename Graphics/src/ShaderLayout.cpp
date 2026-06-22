#include "ShaderLayout.h"
#include <kotono_common/log.h>
#include <spirv-reflect/spirv_reflect.h>

#define KT_LOG_IMPORTANCE_LEVEL_SHADER_LAYOUT ELogImportanceLevel::Low

static constexpr u32 MAX_BINDLESS_TEXTURES{ 8192 };

static const size getTypeSize(const SpvReflectTypeDescription* type)
{
	if (!type)
	{
		KT_LOG(KT_LOG_IMPORTANCE_LEVEL_SHADER_LAYOUT, "Graphics", "type is null");
		return 0;
	}

	if (type->type_flags & SPV_REFLECT_TYPE_FLAG_STRUCT)
	{
		size structSize{ 0 };
		for (u32 i{ 0 }; i < type->member_count; ++i)
		{
			const SpvReflectTypeDescription* member{ &type->members[i] };
			structSize += getTypeSize(member); // Sum up members
		}
		return structSize;  // Return immediately for structs
	}

	size size{ 0 };

	if (type->type_flags & SPV_REFLECT_TYPE_FLAG_BOOL)
	{
		size = 1; // Booleans are typically 1 byte
	}
	else if (type->type_flags & SPV_REFLECT_TYPE_FLAG_INT)
	{
		size = type->traits.numeric.scalar.width / 8;  // Integer size in bytes
	}
	else if (type->type_flags & SPV_REFLECT_TYPE_FLAG_FLOAT)
	{
		size = type->traits.numeric.scalar.width / 8;  // Floating point size in bytes
	}
	if (type->type_flags & SPV_REFLECT_TYPE_FLAG_VECTOR)
	{
		size *= type->traits.numeric.vector.component_count;
	}
	if (type->type_flags & SPV_REFLECT_TYPE_FLAG_MATRIX)
	{
		size *= type->traits.numeric.matrix.column_count;
	}

	return size;
}

void KtShaderLayout::Populate(const std::span<u8> spirvData, const VkShaderStageFlagBits shaderStage)
{
	SpvReflectShaderModule module;
	SpvReflectResult result{ spvReflectCreateShaderModule(spirvData.size() * sizeof(u8), spirvData.data(), &module) };

	if (result != SPV_REFLECT_RESULT_SUCCESS)
	{
		KT_LOG(ELogImportanceLevel::High, "Graphics", "spvReflectCreateShaderModule() returned: {0}", static_cast<u32>(result));
		throw std::runtime_error("couldn't create spirv reflect shader module");
	}

	shaderStages.push_back(shaderStage);

	PopulateDescriptorSets(module, shaderStage);

	PopulatePushConstants(module);

	if (shaderStage == VK_SHADER_STAGE_VERTEX_BIT)
	{
		PopulateVertexInputs(module);
	}

	spvReflectDestroyShaderModule(&module);
}

void KtShaderLayout::PopulateDescriptorSets(const SpvReflectShaderModule& module, const VkShaderStageFlagBits shaderStage)
{
	u32 setCount{ 0 };
	spvReflectEnumerateDescriptorSets(&module, &setCount, nullptr);
	std::vector<SpvReflectDescriptorSet*> sets{ setCount };
	spvReflectEnumerateDescriptorSets(&module, &setCount, sets.data());

	for (auto* set : sets)
	{
		KT_LOG(KT_LOG_IMPORTANCE_LEVEL_SHADER_LAYOUT, "Graphics", "Descriptor Set: {0}, Binding Count: {1}", set->set, set->binding_count);
		for (u32 i{ 0 }; i < set->binding_count; i++)
		{
			const SpvReflectDescriptorBinding* binding{ set->bindings[i] };

			const bool isBindless{ binding->type_description->op == SpvOpTypeRuntimeArray
				|| binding->count == 0 };

			const KtShaderLayout::DescriptorSetLayout::Binding ktBinding{
				.name = binding->name,
				.binding = binding->binding,
				.descriptorType = static_cast<VkDescriptorType>(binding->descriptor_type),
				.descriptorCount = isBindless ? MAX_BINDLESS_TEXTURES : binding->count,
				.shaderStageFlags = static_cast<VkShaderStageFlags>(shaderStage),
				.size = getTypeSize(binding->type_description),
				.isBindless = isBindless,
			};
			descriptorSetLayouts[set->set].bindings.push_back(ktBinding);
		}
	}
}

void KtShaderLayout::PopulatePushConstants(const SpvReflectShaderModule& module)
{
	u32 pushConstantCount{ 0 };
	spvReflectEnumeratePushConstantBlocks(&module, &pushConstantCount, nullptr);
	std::vector<SpvReflectBlockVariable*> push_constants{ pushConstantCount };
	spvReflectEnumeratePushConstantBlocks(&module, &pushConstantCount, push_constants.data());

	for (auto* push_constant : push_constants)
	{
		KT_LOG(KT_LOG_IMPORTANCE_LEVEL_SHADER_LAYOUT, "Graphics", "Push Constant Block: Size {0} bytes", getTypeSize(push_constant->type_description));
	}
}

void KtShaderLayout::PopulateVertexInputs(const SpvReflectShaderModule& module)
{
	u32 inputCount{ 0 };
	spvReflectEnumerateInputVariables(&module, &inputCount, nullptr);
	std::vector<SpvReflectInterfaceVariable*> inputs{ inputCount };
	spvReflectEnumerateInputVariables(&module, &inputCount, inputs.data());

	size offset{ 0 };
	for (auto* input : inputs)
	{
		KT_LOG(KT_LOG_IMPORTANCE_LEVEL_SHADER_LAYOUT, "Graphics", "Vertex Input: Location {0}, Format {1}, Size {2} bytes, Name: {3}", input->location, static_cast<u32>(input->format), getTypeSize(input->type_description), input->name);
		if (input->location != UINT32_MAX)
		{
			const VkVertexInputAttributeDescription vertexInputAttributeDescription{
				.location = input->location,
				.binding = 0,
				.format = static_cast<VkFormat>(input->format),
				.offset = static_cast<u32>(offset),
			};
			vertexInputAttributeDescriptions.push_back(vertexInputAttributeDescription);

			offset += getTypeSize(input->type_description);
		}
	}
	const VkVertexInputBindingDescription vertexInputBindingDescription{
		.binding = 0,
		.stride = static_cast<u32>(offset),
		.inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
	};
	vertexInputBindingDescriptions.push_back(vertexInputBindingDescription);
}
