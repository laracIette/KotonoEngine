#pragma once
#include "InterfaceRenderable.h"
#include <glm/vec2.hpp>
#include <kotono_common/Path.h>
#include <kotono_platform/AllocatedBuffer.h>
#include <vma/vk_mem_alloc.h>
#include <vulkan/vulkan_core.h>
#include <kotono_io/serialize_base.h>
class KtTexture final : public KtInterfaceRenderable
{
public:
    KtTexture(const UPath& path);
    ~KtTexture();

    const UPath& Path() const;

    const glm::uvec2& GetSize() const;

    const VkDescriptorImageInfo& GetDescriptorImageInfo() const;

private:
    void CreateTextureImage();
    void CreateTextureImageView();
    void CreateTextureSampler();

    void DestroyStagingBuffer();

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

    KtAllocatedBuffer stagingBuffer_;

    VkDescriptorImageInfo imageInfo_;
};

template <>
struct USerialize<KtTexture>
{
    void operator()(nlohmann::json& json, const KtTexture* v) const;
};

template <>
struct UDeserialize<KtTexture>
{
    void operator()(const nlohmann::json& json, KtTexture*& v) const;
};
