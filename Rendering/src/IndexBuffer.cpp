#include "IndexBuffer.h"

#include <assert.h>
#include <kotono_platform/Device.h>

static constexpr u32 MAX_INDICES{ 1 << 20 };

UIndexBuffer::UIndexBuffer(UDevice& device)
    : device_{ device }
    , dataBuffer_{}
    , indexCount_{ 0 }
{
}

void UIndexBuffer::Init()
{
    dataBuffer_ = device_.CreateAllocatedBuffer(
          sizeof(u32) * MAX_INDICES
        , VK_BUFFER_USAGE_INDEX_BUFFER_BIT
        | VK_BUFFER_USAGE_TRANSFER_DST_BIT
        , 0
    );
}

void UIndexBuffer::Cleanup() const
{
    device_.CleanupAllocatedBuffer(dataBuffer_);
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

u32 UIndexBuffer::RegisterIndices(std::span<u32 const> indices)
{
    u32 const oldIndex{ indexCount_ };
    u32 const newIndex{ indexCount_ + static_cast<u32>(indices.size()) };
    assert(newIndex <= MAX_INDICES);

    device_.StagingUpload(
          indices.data()
        , indices.size_bytes()
        , dataBuffer_
        , 0
        , indexCount_ * sizeof(u32)
    );

    indexCount_ = newIndex;
    return oldIndex;
}
