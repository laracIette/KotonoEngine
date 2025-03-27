#pragma once
#include <filesystem>
#include <vulkan/vulkan.h>
#include <array>
#include "Shader.h"
#include "ImageTexture.h"
#include "UniformData3D.h"
#include "ObjectData3D.h"
#include "glm_includes.h" 
class KtShader3D final : public KtShader
{
public:
	void Init() override;

	void UpdateUniformBuffer(const KtUniformData3D& uniformData, const uint32_t imageIndex);
	void UpdateObjectBuffer(const std::span<KtObjectData3D> objectDatas, const uint32_t imageIndex);

private:
    KtImageTexture* _imageTexture;
	
	void CreateImageTexture();

	void UpdateDescriptorSet(const uint32_t imageIndex) override;
	void CreateGraphicsPipelines() override;
};

