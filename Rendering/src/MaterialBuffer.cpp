#include "MaterialBuffer.h"
#include <kotono_platform/Context.h>

static constexpr u32 MAX_MATERIALS{ 2048 };

void UMaterialBuffer::Init()
{
    Context.CreateBuffer(dataBuffer_
        , sizeof(Data) * MAX_MATERIALS
        , VK_BUFFER_USAGE_STORAGE_BUFFER_BIT
        | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT
        , VMA_ALLOCATION_CREATE_MAPPED_BIT
        | VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT
    );
}

void UMaterialBuffer::Cleanup() const
{
    vmaDestroyBuffer(Context.GetAllocator(), dataBuffer_.buffer, dataBuffer_.allocation);
}

void UMaterialBuffer::UpdateBuffer(std::span<Data const> datas) const
{
    std::memcpy(dataBuffer_.allocationInfo.pMappedData
        , datas.data()
        , datas.size() * sizeof(Data)
    );
}

VkDeviceAddress UMaterialBuffer::GetAddress() const
{
    return dataBuffer_.bda;
}
