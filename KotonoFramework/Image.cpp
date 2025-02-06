#include "Image.h"

KtImage::KtImage(VkDevice device, VkPhysicalDevice physicalDevice, VmaAllocator allocator) :
    _scale(1.0f, 1.0f),
    _position(0.0f, 0.0f),
    _priority(0)
{
    createImage(device, physicalDevice, allocator);
    createImageView(device);
    createSampler(device);
    createDescriptorSet(device);
}

const glm::uvec2 KtImage::GetBaseSize() const
{
    return { _imageTexture->Size.width, _imageTexture->Size.height };
}

const glm::vec2 KtImage::GetSize() const
{
    return { _imageTexture->Size.width * _scale.x, _imageTexture->Size.height * _scale.y };
}

const glm::vec2& KtImage::GetScale() const
{
    return _scale;
}

const glm::vec2& KtImage::GetPosition() const
{
    return _position;
}

const uint32_t KtImage::GetPriority() const
{
    return _priority;
}

void KtImage::SetSize(const glm::vec2& size)
{
    _scale = { size.x / _imageTexture->Size.width, size.y / _imageTexture->Size.height };
}

void KtImage::SetScale(const glm::vec2& scale)
{
    _scale = scale;
}

void KtImage::SetPosition(const glm::vec2& position)
{
    _position = position;
}

void KtImage::SetPriority(const uint32_t priority)
{
    _priority = priority;
}
