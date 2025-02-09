#pragma once
#include <vulkan/vulkan.h>
#include <stb_image.h>
#include <filesystem>
#include <vk_mem_alloc.h>
class KtImageTexture
{
public:
    // File path of the texture
    const std::filesystem::path Path;
    // Vulkan image handle
    VkImage Image;  
    // Vulkan memory allocation handle
    VmaAllocation Allocation; 
    // Image view for rendering
    VkImageView ImageView; 
    // Sampler for texture filtering
    VkSampler Sampler;
    // Descriptor set for binding image in shaders
    VkDescriptorSet DescriptorSet;  
    // Width and height of the texture
    VkExtent2D Size; 
    // Number of levels of mipmaps
    uint32_t MipLevels;

    KtImageTexture(const std::filesystem::path& path);
    ~KtImageTexture();

private:
    void CreateTextureImage();
    void CreateTextureImageView();
	void CreateTextureSampler();
};