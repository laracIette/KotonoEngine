#include "IndexBuffer.h"

#include <assert.h>
#include <kotono_platform/Device.h>

static constexpr u32 MAX_INDICES{ 1 << 20 };

void UIndexBuffer::Init(UDevice& device)
{
    dataBuffer_ = device.CreateAllocatedBuffer(
          sizeof(u32) * MAX_INDICES
        , VK_BUFFER_USAGE_INDEX_BUFFER_BIT
        | VK_BUFFER_USAGE_TRANSFER_DST_BIT
        , 0
    );
}

void UIndexBuffer::Cleanup(UDevice& device) const
{
    device.CleanupAllocatedBuffer(dataBuffer_);
}

void UIndexBuffer::CmdBind(VkCommandBuffer commandBuffer) const
{
    vkCmdBindIndexBuffer2(commandBuffer
        , dataBuffer_.buffer
        , 0
        , sizeof(u32) * indexCount_
        , VK_INDEX_TYPE_UINT32
    );
}

u32 UIndexBuffer::RegisterIndices(UDevice& device, std::span<u32 const> indices)
{
    u32 const oldIndex{ indexCount_ };
    u32 const newIndex{ indexCount_ + static_cast<u32>(indices.size()) };
    assert(newIndex <= MAX_INDICES);

    device.StagingUpload(
          indices.data()
        , indices.size_bytes()
        , dataBuffer_
        , 0
        , indexCount_ * sizeof(u32)
    );

    indexCount_ = newIndex;
    return oldIndex;
}
