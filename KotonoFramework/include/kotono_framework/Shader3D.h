#pragma once
#include <filesystem>
#include <vulkan/vulkan.h>
#include <array>
#include "frames_in_flight.h"
#include "Shader.h"
#include "ImageTexture.h"
#include "UniformData3D.h"
#include "ObjectData3D.h"
#include "glm_includes.h" 
#include "AllocatedBuffer.h"
class KtShader3D final : public KtShader
{
public:
	void Init() override;
	void Cleanup() override;

	void UpdateUniformBuffer(const KtUniformData3D& uniformData, const uint32_t imageIndex);
	void UpdateObjectBuffer(const std::vector<KtObjectData3D>& objectDatas, const uint32_t imageIndex);

	void CmdBindDescriptorSets(VkCommandBuffer commandBuffer, const uint32_t imageIndex);

private:
    KtImageTexture* _imageTexture;

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

	void CreateImageTexture();

	void CreateDescriptorSetLayouts();
	void CreateDescriptorPools();
	void CreateDescriptorSets();
	void UpdateDescriptorSet(const uint32_t imageIndex);

	void CreateUniformBuffers();
	void CreateObjectBuffers();
	void CreateUniformBuffer(const uint32_t imageIndex);
	void CreateObjectBuffer(const uint32_t imageIndex);
	// Recreates the object buffer when the object count changes
	void SetObjectCount(const VkDeviceSize objectCount, const uint32_t imageIndex);
	const VkDeviceSize GetObjectBufferSize(const uint32_t imageIndex) const;

	void CreateGraphicsPipelines();
};

