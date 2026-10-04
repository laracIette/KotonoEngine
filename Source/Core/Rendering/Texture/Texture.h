#pragma once
#include <glm/ext/vector_uint2.hpp>
#include <kotono_common/Asset.h>
#include <kotono_common/Path.h>
#include <kotono_platform/AllocatedImage.h>
class UDevice;
class ATexture final : public AAsset
{
public:
    ATexture(UPath const& path);

    void Init(UDevice& device);
    void Cleanup(UDevice& device) const;

    glm::uvec2 const& GetSize() const;

    u32 GetIndex() const;
    void SetIndex(u32 index);

    VkImageView GetImageView() const;

private:
    void CreateImage(UDevice& device);

private:
    UAllocatedImage allocatedImage_;
    glm::uvec2 size_;
    u32 index_;
};
