#include "DrawDataBuffer.h"
#include <kotono_platform/Context.h>
#include <kotono_platform/vk_utils.h>
#include <assert.h>

static constexpr u32 MAX_DRAW_DATAS{ 65536 };

void SDrawDataBuffer::Init()
{
    CreateBuffers();
    datas_.resize(MAX_DRAW_DATAS);
}

void SDrawDataBuffer::Cleanup() const
{
    for (const auto& frameData : frameDatas_)
    {
        vmaDestroyBuffer(Context.GetAllocator(), frameData.buffer, frameData.allocation);
    }
}

UDrawDataBufferData* SDrawDataBuffer::RegisterDrawData()
{
    return FindDrawDataSlot();
}

void SDrawDataBuffer::UnregisterDrawData(Data* slot)
{
    freeDataSlots_.push_back(slot);
}

u32 SDrawDataBuffer::GetIndex(const Data* slot) const
{
    return static_cast<u32>(slot - datas_.data());
}

void SDrawDataBuffer::UpdateBuffer(const u32 frameIndex)
{
    std::memcpy(frameDatas_[frameIndex].mapped
        , datas_.data()
        , dataCount_ * sizeof(Data)
    );
}

VkDeviceAddress SDrawDataBuffer::GetAddress(const u32 frameIndex) const
{
    return frameDatas_[frameIndex].bda;
}

void SDrawDataBuffer::CreateBuffers()
{
    for (auto& frameData : frameDatas_)
    {
        const VkBufferCreateInfo bufInfo{
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .size = sizeof(Data) * MAX_DRAW_DATAS,
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

UDrawDataBufferData* SDrawDataBuffer::FindDrawDataSlot()
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
