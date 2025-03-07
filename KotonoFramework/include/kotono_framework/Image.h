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

    VkCommandBuffer commandBuffer;  // Command buffer for rendering the image


    void createImage(VkDevice device, VkPhysicalDevice physicalDevice, VmaAllocator allocator)
    {
        // Create Vulkan image, allocate memory, and bind memory (use VMA for allocation)
    }

    void createImageView(VkDevice device)
    {
        // Create image view from the Vulkan image
    }

    void createSampler(VkDevice device)
    {
        // Create Vulkan sampler for the image (for texture filtering, etc.)
    }

    void createDescriptorSet(VkDevice device)
    {
        // Set up descriptor set to bind the image to shaders
    }

    void cleanup()
    {
        // Cleanup Vulkan resources (image, memory, view, sampler, etc.)
    }

    // Method to render the image with proper queue handling
    void render(VkCommandBuffer commandBuffer, VkQueue queue)
    {
        // Submit commands for rendering the image here
        // Ensure the image is correctly presented to the queue
    }
};
