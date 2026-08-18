#include "ParametersBuffer.h"
#include <kotono_platform/Context.h>

static constexpr u32 MAX_PARAMETERS{ 65536 };

void UParametersBuffer::Init()
{
    CreateBuffers();
}

void UParametersBuffer::Cleanup() const
{
    vmaDestroyBuffer(Context.GetAllocator(), dataBuffer_.buffer, dataBuffer_.allocation);
}

void UParametersBuffer::UpdateBuffer(std::span<Data const> datas) const
{
    std::memcpy(dataBuffer_.allocationInfo.pMappedData
        , datas.data()
        , datas.size() * sizeof(Data)
    );
}

VkDeviceAddress UParametersBuffer::GetAddress() const
{
    return dataBuffer_.bda;
}

void UParametersBuffer::CreateBuffers()
{
    Context.CreateBuffer(dataBuffer_
        , sizeof(Data) * MAX_PARAMETERS
        , VK_BUFFER_USAGE_STORAGE_BUFFER_BIT
        | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT
        , VMA_ALLOCATION_CREATE_MAPPED_BIT
        | VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT
    );
}
