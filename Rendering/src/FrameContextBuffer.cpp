#include "FrameContextBuffer.h"
#include <kotono_platform/Context.h>

void UFrameContextBuffer::Init()
{
    CreateBuffers();
}

void UFrameContextBuffer::Cleanup() const
{
    vmaDestroyBuffer(Context.GetAllocator(), dataBuffer_.buffer, dataBuffer_.allocation);
}

void UFrameContextBuffer::UpdateBuffer(
      UFrameContextSceneView const& sceneView
    , UFrameContextAddresses const& addresses
    , UFrameContextTargets const& targets
    , u32 directionalLightCount
    , u32 pointLightCount
    , u32 samplerIndex
) const
{    
    const auto viewProj{ sceneView.proj * sceneView.view };

    const Data data{
        .view = sceneView.view,
        .proj = sceneView.proj,
        .invProj = glm::inverse(sceneView.proj),
        .viewProj = viewProj,
        .invViewProj = glm::inverse(viewProj),
        .viewPos = sceneView.viewPos,

        .windowSize = sceneView.extent,

        .time = sceneView.time,

        .drawDataBufferAddress = addresses.drawDataBufferAddress,
        .materialBufferAddress = addresses.materialBufferAddress,
        .transformBufferAddress = addresses.transformBufferAddress,
        .parametersBufferAddress = addresses.parametersBufferAddress,

        .directionalLightBufferAddress = addresses.directionalLightBufferAddress,
        .pointLightBufferAddress = addresses.pointLightBufferAddress,
        .directionalLightCount = directionalLightCount,
        .pointLightCount = pointLightCount,

        .clusterAABBBufferAddress = addresses.clusterAABBBufferAddress,
        .clusterGridBufferAddress = addresses.clusterGridBufferAddress,
        .lightIndexBufferAddress = addresses.lightIndexBufferAddress,
        .lightCounterBufferAddress = addresses.lightCounterBufferAddress,

        .gBufferDepth = targets.gBufferDepth,
        .gBufferAlbedo = targets.gBufferAlbedo,
        .gBufferNormal = targets.gBufferNormal,
        .gBufferORM = targets.gBufferORM,
        .gBufferSampler = samplerIndex,

        .colorTarget = targets.colorTarget,
        .colorSampler = samplerIndex,
    };

    std::memcpy(dataBuffer_.allocationInfo.pMappedData
        , &data
        , sizeof(Data)
    );
}

VkDeviceAddress UFrameContextBuffer::GetAddress() const
{
    return dataBuffer_.bda;
}

void UFrameContextBuffer::CreateBuffers()
{
    Context.CreateBuffer(dataBuffer_
        , sizeof(Data)
        , VK_BUFFER_USAGE_STORAGE_BUFFER_BIT
        | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT
        , VMA_ALLOCATION_CREATE_MAPPED_BIT
        | VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT
    );
}
