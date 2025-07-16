#pragma once
#include <vulkan/vulkan_core.h>
#include <filesystem>
#include <vma/vk_mem_alloc.h>
#include "AllocatedBuffer.h"
#include "glm_includes.h"
#include "Renderable2D.h"
class KtImageTexture final : public KtRenderable2D
{
public:
    KtImageTexture(const std::filesystem::path& path);

    void Init();
    void Cleanup() const;

    const std::filesystem::path& GetPath() const;
    const glm::uvec2& GetSize() const;

    const VkDescriptorImageInfo& GetDescriptorImageInfo() const;

private:
    // File path of the texture
    const std::filesystem::path path_;

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
    uint32_t mipLevels_;

    KtAllocatedBuffer stagingBuffer_;

    VkDescriptorImageInfo imageInfo_;

    void CreateTextureImage();
    void CreateTextureImageView();
	void CreateTextureSampler();

    void DestroyStagingBuffer();
};