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
};

