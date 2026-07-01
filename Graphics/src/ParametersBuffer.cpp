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
        vmaDestroyBuffer(Context.GetAllocator(), frameData.buffer, frameData.allocation);
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
    std::memcpy(frameDatas_[frameIndex].mapped
        , datas_.data()
        , dataCount_ * sizeof(Data)
    );
}

VkDeviceAddress GParametersBuffer::GetAddress(const u32 frameIndex) const
{
    return frameDatas_[frameIndex].bda;
}

void GParametersBuffer::CreateBuffers()
{
    for (auto& frameData : frameDatas_)
    {
        const VkBufferCreateInfo bufInfo{
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .size = sizeof(Data) * MAX_PARAMETERS,
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

        frameData.mapped = static_cast<Data*>(allocationInfo.pMappedData);

        const VkBufferDeviceAddressInfo addrInfo{
            .sType = VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO,
            .buffer = frameData.buffer,
        };
        frameData.bda = vkGetBufferDeviceAddress(Context.GetDevice(), &addrInfo);
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
