#pragma once
#include "InterfaceRenderable.h"
#include <glm/vec2.hpp>
#include <kotono_common/Path.h>
#include <kotono_platform/AllocatedBuffer.h>
#include <vma/vk_mem_alloc.h>
#include <vulkan/vulkan_core.h>
class UTexture final : public KtInterfaceRenderable
{
public:
    UTexture(const UPath& path);
    ~UTexture();

    const UPath& Path() const;

    const glm::uvec2& GetSize() const;

    const VkDescriptorImageInfo& GetDescriptorImageInfo() const;

    u32 GetIndex() const;

private:
    void CreateImage();
    void CreateImageView();
    void CreateSampler();

    void DestroyStagingBuffer() const;

private:
    // File path of the texture
    const UPath path_;

    // Vulkan image handle
    VkImage image_;
    // Vulkan memory allocation handle
    VmaAllocation allocation_;
    // Image view for rendering
    VkImageView imageView_;
    // Sampler for texture filtering
    VkSampler sampler_;
    // Descriptor set for binding image in shaders
    VkDescriptorSet descriptorSet_;
    // Width and height of the texture
    glm::uvec2 size_;
    // Number of levels of mipmaps
    u32 mipLevels_;

    u32 index_;

    KtAllocatedBuffer stagingBuffer_;

    VkDescriptorImageInfo imageInfo_; // TODO: yoink
};
