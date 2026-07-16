#include "IndexBuffer.h"
#include <assert.h>
#include <kotono_platform/Context.h>

static constexpr u32 MAX_INDICES{ 1 << 20 };

void GIndexBuffer::Init()
{
    Context.CreateBuffer(dataBuffer_
        , sizeof(u32) * MAX_INDICES
        , VK_BUFFER_USAGE_INDEX_BUFFER_BIT
        | VK_BUFFER_USAGE_TRANSFER_DST_BIT
        , 0
    );
}

void GIndexBuffer::Cleanup() const
{
    vmaDestroyBuffer(Context.GetAllocator(), dataBuffer_.buffer, dataBuffer_.allocation);
}

void GIndexBuffer::CmdBind(VkCommandBuffer commandBuffer) const
{
    vkCmdBindIndexBuffer2(commandBuffer
        , dataBuffer_.buffer
        , 0
        , sizeof(u32) * indexCount_
        , VK_INDEX_TYPE_UINT32
    );
}

u32 GIndexBuffer::RegisterIndices(const std::span<u32> indices)
{
    const u32 oldIndex{ indexCount_ };
    const u32 newIndex{ indexCount_ + static_cast<u32>(indices.size()) };
    assert(newIndex <= MAX_INDICES);

    Context.StagingUpload(indices.data()
        , sizeof(u32) * indices.size()
        , dataBuffer_.buffer
        , indexCount_ * sizeof(u32)
    );

    indexCount_ = newIndex;
    return oldIndex;
}
