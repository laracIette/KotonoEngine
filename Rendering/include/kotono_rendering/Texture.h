#pragma once
#include <glm/ext/vector_uint2.hpp>
#include <kotono_common/Asset.h>
#include <kotono_common/Path.h>
#include <kotono_platform/AllocatedBuffer.h>
#include <kotono_platform/AllocatedImage.h>
class ATexture final : public AAsset
{
public:
    ATexture(UPath const& path);
    ~ATexture() override;

    glm::uvec2 const& GetSize() const;

    u32 GetIndex() const;
    void SetIndex(u32 index);

    VkImageView GetImageView() const;

private:
    void CreateImage();

    void DestroyStagingBuffer() const;

private:
    UAllocatedImage allocatedImage_;
    UAllocatedBuffer stagingBuffer_;

    glm::uvec2 size_;

    u32 index_;
};
