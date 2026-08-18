#pragma once
#include <glm/ext/vector_uint2.hpp>
#include <kotono_common/Asset.h>
#include <kotono_common/Path.h>
#include <kotono_platform/AllocatedBuffer.h>
#include <kotono_platform/AllocatedImage.h>
class ATexture final : public AAsset
{
public:
    ATexture(const UPath& path);
    ~ATexture() override;

    const glm::uvec2& GetSize() const;

    u32 GetIndex() const;

private:
    void CreateImage();

    void DestroyStagingBuffer() const;

private:
    UAllocatedImage allocatedImage_;
    UAllocatedBuffer stagingBuffer_;

    glm::uvec2 size_;
    u32 mipLevels_;

    u32 index_;
};
