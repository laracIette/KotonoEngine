#pragma once
#include "frames_in_flight.h"
#include <kotono_common/types.h>
#include <glm/mat4x4.hpp>
#include <vector>
#include <vma/vk_mem_alloc.h>
#include <vulkan/vulkan_core.h>
class STransformBuffer final
{
public:
    struct Transform
    {
        glm::mat4 modelMatrix;
        glm::mat4 normalMatrix;
    };

    struct FrameData
    {
        VkBuffer buffer;
        VmaAllocation allocation;
        Transform* mapped;
        VkDeviceAddress bda;
    };

public:
    void Init();
    void Cleanup() const;

    u32 RegisterTransform();
    void UnregisterTransform(const u32 index);
    void UpdateTransform(const u32 index, const Transform& transform);

    void UpdateTransforms(const u32 frameIndex);
    VkDeviceAddress GetAddress(const u32 frameIndex) const;

private:
    void CreateBuffers();
    u32 FindTransformSlot();

private:
    KtFramesInFlightArray<FrameData> frameDatas_;

    std::vector<Transform> transforms_;
    std::vector<u32> freeTransformSlots_;
    u32 transformCount_;
};

inline STransformBuffer TransformBuffer;
