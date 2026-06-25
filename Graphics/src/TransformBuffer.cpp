#include "TransformBuffer.h"
#include <kotono_platform/Context.h>
#include <kotono_platform/vk_utils.h>

static constexpr u32 MAX_TRANSFORMS{ 65536 };

void STransformBuffer::Init()
{
    CreateBuffers();
    transforms_.resize(MAX_TRANSFORMS);
}

void STransformBuffer::Cleanup() const
{
    for (const auto& frameData : frameDatas_)
    {
        vmaDestroyBuffer(Context.GetAllocator(), frameData.buffer, frameData.allocation);
    }
}

u32 STransformBuffer::RegisterTransform(const Transform& transform)
{
    const auto slot{ FindTransformSlot() };
    transforms_[slot] = transform;
    return slot;
}

void STransformBuffer::UpdateTransform(const Transform& transform, const u32 index)
{
    transforms_[index] = transform;
}

void STransformBuffer::UnregisterTransform(const u32 index)
{
    freeTransformSlots_.push_back(index);
}

void STransformBuffer::UpdateTransforms(const u32 frameIndex)
{
    std::memcpy(frameDatas_[frameIndex].mapped
        , transforms_.data()
        , transformCount_ * sizeof(Transform)
    );
}

VkDeviceAddress STransformBuffer::GetAddress(const u32 frameIndex) const
{
    return frameDatas_[frameIndex].bda;
}

void STransformBuffer::CreateBuffers()
{
    for (auto& frameData : frameDatas_)
    {
        const VkBufferCreateInfo bufInfo{
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .size = sizeof(Transform) * MAX_TRANSFORMS,
        .usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT
            | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT,
        };

        // HOST_VISIBLE so CPU can write directly each frame
        const VmaAllocationCreateInfo allocInfo{
            .flags = VMA_ALLOCATION_CREATE_MAPPED_BIT
                | VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT,
            .usage = VMA_MEMORY_USAGE_AUTO,
        };

        VmaAllocationInfo allocationInfo;
        VK_CHECK_THROW(
            vmaCreateBuffer(Context.GetAllocator()
                , &bufInfo
                , &allocInfo
                , &frameData.buffer
                , &frameData.allocation
                , &allocationInfo
            ),
            "failed to create buffer with VMA!"
        );

        frameData.mapped = static_cast<Transform*>(allocationInfo.pMappedData);

        const VkBufferDeviceAddressInfo addrInfo{
            .sType = VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO,
            .buffer = frameData.buffer,
        };
        frameData.bda = vkGetBufferDeviceAddress(Context.GetDevice(), &addrInfo);
    }
}

u32 STransformBuffer::FindTransformSlot()
{
    if (!freeTransformSlots_.empty())
    {
        const u32 slot{ freeTransformSlots_.back() };
        freeTransformSlots_.pop_back();
        return slot;
    }
    assert(transformCount_ < MAX_TRANSFORMS);
    return transformCount_++;
}
