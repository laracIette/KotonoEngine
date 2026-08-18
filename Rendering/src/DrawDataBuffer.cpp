#include "DrawDataBuffer.h"
#include <kotono_platform/Context.h>

static constexpr u32 MAX_DRAW_DATAS{ 65536 };

void UDrawDataBuffer::Init()
{
    CreateBuffers();
}

void UDrawDataBuffer::Cleanup() const
{
    vmaDestroyBuffer(Context.GetAllocator(), dataBuffer_.buffer, dataBuffer_.allocation);
}

void UDrawDataBuffer::UpdateBuffer(std::span<Data const> datas) const
{
    std::memcpy(dataBuffer_.allocationInfo.pMappedData
        , datas.data()
        , datas.size() * sizeof(Data)
    );
}

VkDeviceAddress UDrawDataBuffer::GetAddress() const
{
    return dataBuffer_.bda;
}

void UDrawDataBuffer::CreateBuffers()
{
    Context.CreateBuffer(dataBuffer_
        , sizeof(Data) * MAX_DRAW_DATAS
        , VK_BUFFER_USAGE_STORAGE_BUFFER_BIT
        | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT
        , VMA_ALLOCATION_CREATE_MAPPED_BIT
        | VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT
    );
}
