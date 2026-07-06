#include "FrameContextBuffer.h"
#include "GPUBuffers.h"
#include "DrawDataBuffer.h"
#include "LightBuffer.h"
#include "MaterialBuffer.h"
#include "ParametersBuffer.h"
#include "TransformBuffer.h"
#include <kotono_platform/Context.h>
#include <kotono_platform/vk_utils.h>
#include <kotono_timing/Clock.h>

void GFrameContextBuffer::Init()
{
    CreateBuffers();
}

void GFrameContextBuffer::Cleanup() const
{
    for (const auto& frameData : frameDatas_)
    {
        vmaDestroyBuffer(Context.GetAllocator(), frameData.buffer, frameData.allocation);
    }
}

void GFrameContextBuffer::SetCameraData(const CameraData& cameraData)
{
    cameraData_ = cameraData;
}

void GFrameContextBuffer::UpdateBuffer(const u32 frameIndex)
{
    const Data data{
        .view = cameraData_.view,
        .proj = cameraData_.proj,
        .invProj = glm::inverse(cameraData_.proj),
        .viewProj = cameraData_.viewProj,
        .invViewProj = glm::inverse(cameraData_.viewProj),
        .viewPos = glm::vec4{ cameraData_.viewPos, 0.0f },

        .time = SClock::Now(),

        .drawDataBufferAddress = DrawDataBuffer.GetAddress(frameIndex),
        .materialBufferAddress = MaterialBuffer.GetAddress(),
        .transformBufferAddress = TransformBuffer.GetAddress(frameIndex),
        .parametersBufferAddress = ParametersBuffer.GetAddress(frameIndex),

        .lightBufferAddress = LightBuffer.GetAddress(),
        .lightCount = LightBuffer.GetLightCount(),

        .clusterAABBBufferAddress = GPUBuffers.GetClusterAABBAddress(),
        .clusterGridBufferAddress = GPUBuffers.GetClusterGridAddress(),
        .lightIndexBufferAddress = GPUBuffers.GetLightIndexAddress(),
        .lightCounterBufferAddress = GPUBuffers.GetLightCounterAddress(),
    };

    std::memcpy(frameDatas_[frameIndex].mapped
        , &data
        , sizeof(Data)
    );
}

VkDeviceAddress GFrameContextBuffer::GetAddress(const u32 frameIndex) const
{
    return frameDatas_[frameIndex].bda;
}

void GFrameContextBuffer::CreateBuffers()
{
    for (auto& frameData : frameDatas_)
    {
        const VkBufferCreateInfo bufInfo{
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .size = sizeof(Data),
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

        frameData.mapped = static_cast<Data*>(allocationInfo.pMappedData);

        const VkBufferDeviceAddressInfo addrInfo{
            .sType = VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO,
            .buffer = frameData.buffer,
        };
        frameData.bda = vkGetBufferDeviceAddress(Context.GetDevice(), &addrInfo);
    }
}
