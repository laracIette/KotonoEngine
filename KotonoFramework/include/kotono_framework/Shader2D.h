#pragma once
#include <filesystem>
#include <array>
#include <vulkan/vulkan.h>
#include "Shader.h"
#include "ImageTexture.h"
#include "UniformData2D.h"
#include "ObjectData2D.h"
#include "glm_includes.h" 
class KtShader2D final : public KtShader
{
public:
	void Init() override;

	void UpdateUniformBuffer(const KtUniformData2D& uniformData, const uint32_t imageIndex);
	void UpdateObjectBuffer(const std::span<KtObjectData2D> objectDatas, const uint32_t imageIndex);

private:
    KtImageTexture* _imageTexture;

	void CreateImageTexture();

	void CreateDescriptorSetLayouts() override;

	void UpdateDescriptorSet(const uint32_t imageIndex) override;
	void CreateGraphicsPipelines() override;
};

