#pragma once
#include <glm/vec2.hpp>
#include <kotono_common/Path.h>
#include <kotono_platform/AllocatedBuffer.h>
#include <vma/vk_mem_alloc.h>
#include <vulkan/vulkan_core.h>
class UTexture final
{
public:
    UTexture(const UPath& path);
    ~UTexture();

    const UPath& Path() const;

    const glm::uvec2& GetSize() const;

    u32 GetIndex() const;

private:
    void CreateImage();
    void CreateImageView();

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
    // Descriptor set for binding image in shaders
    VkDescriptorSet descriptorSet_;
    // Width and height of the texture
    glm::uvec2 size_;
    // Number of levels of mipmaps
    u32 mipLevels_;

    u32 index_;

    UAllocatedBuffer stagingBuffer_;
};
