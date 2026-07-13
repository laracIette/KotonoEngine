#include "TransformBuffer.h"
#include <kotono_platform/Context.h>
#include <kotono_platform/vk_utils.h>

static constexpr u32 MAX_TRANSFORMS{ 65536 };

void GTransformBuffer::Init()
{
    CreateBuffers();
    datas_.resize(MAX_TRANSFORMS);
}

void GTransformBuffer::Cleanup() const
{
    for (const auto& frameData : frameDatas_)
    {
        vmaDestroyBuffer(Context.GetAllocator(), frameData.dataBuffer.buffer, frameData.dataBuffer.allocation);
    }
}

UTransformBufferData* GTransformBuffer::RegisterTransform()
{
    return FindTransformSlot();
}

void GTransformBuffer::UnregisterTransform(Data* slot)
{
    freeDataSlots_.push_back(slot);
}

u32 GTransformBuffer::GetIndex(const Data* slot) const
{
    return static_cast<u32>(slot - datas_.data());
}

void GTransformBuffer::UpdateBuffer(const u32 frameIndex)
{
    std::memcpy(frameDatas_[frameIndex].dataBuffer.allocationInfo.pMappedData
        , datas_.data()
        , dataCount_ * sizeof(Data)
    );
}

VkDeviceAddress GTransformBuffer::GetAddress(const u32 frameIndex) const
{
    return frameDatas_[frameIndex].dataBuffer.bda;
}

void GTransformBuffer::CreateBuffers()
{
    for (auto& frameData : frameDatas_)
    {
        Context.CreateBuffer(frameData.dataBuffer
            , sizeof(Data) * MAX_TRANSFORMS
            , VK_BUFFER_USAGE_STORAGE_BUFFER_BIT
            | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT
            , VMA_ALLOCATION_CREATE_MAPPED_BIT
            | VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT
        );
    }
}

UTransformBufferData* GTransformBuffer::FindTransformSlot()
{
    if (!freeDataSlots_.empty())
    {
        Data* slot{ freeDataSlots_.back() };
        freeDataSlots_.pop_back();
        return slot;
    }
    assert(dataCount_ < MAX_TRANSFORMS);
    return &datas_[dataCount_++];
}
