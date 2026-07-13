#pragma once
#include <glm/vec2.hpp>
#include <kotono_common/Path.h>
#include <kotono_platform/AllocatedBuffer.h>
#include <kotono_platform/AllocatedImage.h>
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

    UAllocatedImage allocatedImage_;
    UAllocatedBuffer stagingBuffer_;

    glm::uvec2 size_;
    u32 mipLevels_;

    u32 index_;
};
