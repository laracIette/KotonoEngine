#include "ParametersBuffer.h"
#include <kotono_platform/Context.h>
#include <kotono_platform/vk_utils.h>

static constexpr u32 MAX_PARAMETERS{ 65536 };

void GParametersBuffer::Init()
{
    CreateBuffers();
    datas_.resize(MAX_PARAMETERS);
}

void GParametersBuffer::Cleanup() const
{
    for (const auto& frameData : frameDatas_)
    {
        vmaDestroyBuffer(Context.GetAllocator(), frameData.dataBuffer.buffer, frameData.dataBuffer.allocation);
    }
}

UParametersBufferData* GParametersBuffer::RegisterParameters()
{
    return FindParametersSlot();
}

void GParametersBuffer::UnregisterParameters(Data* slot)
{
    freeDataSlots_.push_back(slot);
}

u32 GParametersBuffer::GetIndex(const Data* slot) const
{
    return static_cast<u32>(slot - datas_.data());
}

void GParametersBuffer::UpdateBuffer(const u32 frameIndex)
{
    std::memcpy(frameDatas_[frameIndex].dataBuffer.allocationInfo.pMappedData
        , datas_.data()
        , dataCount_ * sizeof(Data)
    );
}

VkDeviceAddress GParametersBuffer::GetAddress(const u32 frameIndex) const
{
    return frameDatas_[frameIndex].dataBuffer.bda;
}

void GParametersBuffer::CreateBuffers()
{
    for (auto& frameData : frameDatas_)
    {
        Context.CreateBuffer(frameData.dataBuffer
            , sizeof(Data) * MAX_PARAMETERS
            , VK_BUFFER_USAGE_STORAGE_BUFFER_BIT
            | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT
            , VMA_ALLOCATION_CREATE_MAPPED_BIT
            | VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT
        );
    }
}

UParametersBufferData* GParametersBuffer::FindParametersSlot()
{
    if (!freeDataSlots_.empty())
    {
        Data* slot{ freeDataSlots_.back() };
        freeDataSlots_.pop_back();
        return slot;
    }
    assert(dataCount_ < MAX_PARAMETERS);
    return &datas_[dataCount_++];
}
