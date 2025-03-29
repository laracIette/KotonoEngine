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
};

