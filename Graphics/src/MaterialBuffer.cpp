#include "MaterialBuffer.h"
#include <assert.h>
#include <kotono_platform/Context.h>

static constexpr u32 MAX_MATERIALS{ 4096 };

void GMaterialBuffer::Init()
{
    Context.CreateBuffer(dataBuffer_
        , sizeof(Material) * MAX_MATERIALS
        , VK_BUFFER_USAGE_STORAGE_BUFFER_BIT
        | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT
        | VK_BUFFER_USAGE_TRANSFER_DST_BIT
        , 0
    );
}

void GMaterialBuffer::Cleanup() const
{
    vmaDestroyBuffer(Context.GetAllocator(), dataBuffer_.buffer, dataBuffer_.allocation);
}

u32 GMaterialBuffer::RegisterMaterial(const Material& material)
{
    Context.StagingUpload(&material
        , sizeof(Material)
        , dataBuffer_.buffer
        , materialCount_ * sizeof(Material)
    );
    assert(materialCount_ < MAX_MATERIALS);
    return materialCount_++;
}

VkDeviceAddress GMaterialBuffer::GetAddress() const
{
    return dataBuffer_.bda;
}
