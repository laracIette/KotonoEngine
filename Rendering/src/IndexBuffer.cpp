#include "IndexBuffer.h"
#include <assert.h>
#include <kotono_platform/Context.h>

static constexpr u32 MAX_INDICES{ 1 << 20 };

void UIndexBuffer::Init()
{
    Context.CreateBuffer(dataBuffer_
        , sizeof(u32) * MAX_INDICES
        , VK_BUFFER_USAGE_INDEX_BUFFER_BIT
        | VK_BUFFER_USAGE_TRANSFER_DST_BIT
        , 0
    );
}

void UIndexBuffer::Cleanup() const
{
    vmaDestroyBuffer(Context.GetAllocator(), dataBuffer_.buffer, dataBuffer_.allocation);
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

    Context.StagingUpload(indices.data()
        , sizeof(u32) * indices.size()
        , dataBuffer_.buffer
        , indexCount_ * sizeof(u32)
    );

    indexCount_ = newIndex;
    return oldIndex;
}
