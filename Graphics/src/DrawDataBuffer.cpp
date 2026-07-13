#include "DrawDataBuffer.h"
#include <kotono_platform/Context.h>
#include <kotono_platform/vk_utils.h>
#include <assert.h>

static constexpr u32 MAX_DRAW_DATAS{ 65536 };

void GDrawDataBuffer::Init()
{
    CreateBuffers();
    datas_.resize(MAX_DRAW_DATAS);
}

void GDrawDataBuffer::Cleanup() const
{
    for (const auto& frameData : frameDatas_)
    {
        vmaDestroyBuffer(Context.GetAllocator(), frameData.dataBuffer.buffer, frameData.dataBuffer.allocation);
    }
}

UDrawDataBufferData* GDrawDataBuffer::RegisterDrawData()
{
    return FindDrawDataSlot();
}

void GDrawDataBuffer::UnregisterDrawData(Data* slot)
{
    freeDataSlots_.push_back(slot);
}

u32 GDrawDataBuffer::GetIndex(const Data* slot) const
{
    return static_cast<u32>(slot - datas_.data());
}

void GDrawDataBuffer::UpdateBuffer(const u32 frameIndex)
{
    std::memcpy(frameDatas_[frameIndex].dataBuffer.allocationInfo.pMappedData
        , datas_.data()
        , dataCount_ * sizeof(Data)
    );
}

VkDeviceAddress GDrawDataBuffer::GetAddress(const u32 frameIndex) const
{
    return frameDatas_[frameIndex].dataBuffer.bda;
}

void GDrawDataBuffer::CreateBuffers()
{
    for (auto& frameData : frameDatas_)
    {
        Context.CreateBuffer(frameData.dataBuffer
            , sizeof(Data) * MAX_DRAW_DATAS
            , VK_BUFFER_USAGE_STORAGE_BUFFER_BIT
            | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT
            , VMA_ALLOCATION_CREATE_MAPPED_BIT
            | VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT
        );
    }
}

UDrawDataBufferData* GDrawDataBuffer::FindDrawDataSlot()
{
    if (!freeDataSlots_.empty())
    {
        Data* slot{ freeDataSlots_.back() };
        freeDataSlots_.pop_back();
        return slot;
    }
    assert(dataCount_ < MAX_DRAW_DATAS);
    return &datas_[dataCount_++];
}
