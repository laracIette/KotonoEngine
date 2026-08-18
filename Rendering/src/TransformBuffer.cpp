#include "TransformBuffer.h"
#include <kotono_platform/Context.h>

static constexpr u32 MAX_TRANSFORMS{ 65536 };

void UTransformBuffer::Init()
{
    CreateBuffers();
}

void UTransformBuffer::Cleanup() const
{
    vmaDestroyBuffer(Context.GetAllocator(), dataBuffer_.buffer, dataBuffer_.allocation);
}

void UTransformBuffer::UpdateBuffer(std::span<Data const> datas) const
{
    std::memcpy(dataBuffer_.allocationInfo.pMappedData
        , datas.data()
        , datas.size() * sizeof(Data)
    );
}

VkDeviceAddress UTransformBuffer::GetAddress() const
{
    return dataBuffer_.bda;
}

void UTransformBuffer::CreateBuffers()
{
    Context.CreateBuffer(dataBuffer_
        , sizeof(Data) * MAX_TRANSFORMS
        , VK_BUFFER_USAGE_STORAGE_BUFFER_BIT
        | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT
        , VMA_ALLOCATION_CREATE_MAPPED_BIT
        | VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT
    );
}
