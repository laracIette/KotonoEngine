#include "ParametersBuffer.h"
#include <kotono_platform/Context.h>
#include <kotono_platform/vk_utils.h>

static constexpr u32 MAX_PARAMETERS{ 65536 };

void SParametersBuffer::Init()
{
    CreateBuffers();
    parameters_.resize(MAX_PARAMETERS);
}

void SParametersBuffer::Cleanup() const
{
    for (const auto& frameData : frameDatas_)
    {
        vmaDestroyBuffer(Context.GetAllocator(), frameData.buffer, frameData.allocation);
    }
}

u32 SParametersBuffer::RegisterParameters()
{
    return FindParametersSlot();
}

void SParametersBuffer::UpdateParameters(const u32 index, const Parameters& parameters)
{
    parameters_[index] = parameters;
}

void SParametersBuffer::UnregisterParameters(const u32 index)
{
    freeParametersSlots_.push_back(index);
}

void SParametersBuffer::UpdateBuffer(const u32 frameIndex)
{
    std::memcpy(frameDatas_[frameIndex].mapped
        , parameters_.data()
        , parametersCount_ * sizeof(Parameters)
    );
}

VkDeviceAddress SParametersBuffer::GetAddress(const u32 frameIndex) const
{
    return frameDatas_[frameIndex].bda;
}

void SParametersBuffer::CreateBuffers()
{
    for (auto& frameData : frameDatas_)
    {
        const VkBufferCreateInfo bufInfo{
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .size = sizeof(Parameters) * MAX_PARAMETERS,
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

        frameData.mapped = static_cast<Parameters*>(allocationInfo.pMappedData);

        const VkBufferDeviceAddressInfo addrInfo{
            .sType = VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO,
            .buffer = frameData.buffer,
        };
        frameData.bda = vkGetBufferDeviceAddress(Context.GetDevice(), &addrInfo);
    }
}

u32 SParametersBuffer::FindParametersSlot()
{
    if (!freeParametersSlots_.empty())
    {
        const u32 slot{ freeParametersSlots_.back() };
        freeParametersSlots_.pop_back();
        return slot;
    }
    assert(parametersCount_ < MAX_PARAMETERS);
    return parametersCount_++;
}
