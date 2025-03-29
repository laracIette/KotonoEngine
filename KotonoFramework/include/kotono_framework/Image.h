#pragma once
#include <vulkan/vulkan.h>
#include <vma/vk_mem_alloc.h>
#include "glm_includes.h"
#include "ImageTexture.h"
#include "Shader.h"
class KtImage
{
public:    
    void AddToRenderQueue2D(const glm::mat4& modelMatrix) const;

    KtShader* GetShader() const;
    void SetShader(KtShader* shader);

private:
    KtShader* _shader;
};
