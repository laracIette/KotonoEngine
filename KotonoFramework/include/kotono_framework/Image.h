#pragma once
#include <vulkan/vulkan.h>
#include <vma/vk_mem_alloc.h>
#include "glm_includes.h"
#include "ImageTexture.h"
class KtImage
{
public:
    KtImage(VkDevice device, VkPhysicalDevice physicalDevice, VmaAllocator allocator);

    const glm::uvec2 GetBaseSize() const;
    const glm::vec2 GetSize() const;
    const glm::vec2& GetScale() const;
    const glm::vec2& GetPosition() const;
    const uint32_t GetPriority() const;

    void SetSize(const glm::vec2& size);
    void SetScale(const glm::vec2& scale);
    void SetPosition(const glm::vec2& position);
    void SetPriority(const uint32_t priority);

private:

    KtImageTexture* _imageTexture;

    glm::vec2 _scale;
    glm::vec2 _position; 
    uint32_t _priority;  

};
