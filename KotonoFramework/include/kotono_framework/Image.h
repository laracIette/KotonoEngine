#pragma once
#include <vulkan/vulkan.h>
#include <vma/vk_mem_alloc.h>
#include "glm_includes.h"
#include "ImageTexture.h"
#include "Shader2D.h"
class KtImage
{
public:    
    void AddToRenderQueue2D(const glm::mat4& modelMatrix) const;

    KtShader2D* GetShader() const;
    void SetShader(KtShader2D* shader);

private:
    KtShader2D* _shader;
};
