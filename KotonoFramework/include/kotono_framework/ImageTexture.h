#pragma once
#include <vulkan/vulkan.h>
#include <stbimage/stb_image.h>
#include <filesystem>
#include <vma/vk_mem_alloc.h>
class KtImageTexture
{
public:
    KtImageTexture(const std::filesystem::path& path);

    void Init();
    void Cleanup() const;

    const std::filesystem::path& GetPath() const;

    const VkDescriptorImageInfo GetDescriptorImageInfo() const;

private:
    // File path of the texture
    const std::filesystem::path _path;

    // Vulkan image handle
    VkImage _image;
    // Vulkan memory allocation handle
    VmaAllocation _allocation;
    // Image view for rendering
    VkImageView _imageView;
    // Sampler for texture filtering
    VkSampler _sampler;
    // Descriptor set for binding image in shaders
    VkDescriptorSet _descriptorSet;
    // Width and height of the texture
    VkExtent2D _size;
    // Number of levels of mipmaps
    uint32_t _mipLevels;

    void CreateTextureImage();
    void CreateTextureImageView();
	void CreateTextureSampler();
};