#include "FrameContextBuffer.h"
#include "PipelineResourceManager.h"
#include <kotono_platform/Context.h>
#include <ranges>

void UFrameContextBuffer::Init()
{
    CreateBuffers();
}

void UFrameContextBuffer::Cleanup() const
{
    for (const auto& frameData : frameDatas_)
    {
        vmaDestroyBuffer(Context.GetAllocator(), frameData.dataBuffer.buffer, frameData.dataBuffer.allocation);
    }
}

void UFrameContextBuffer::RegisterTextures(GPipelineResourceManager& pipelineResourceManager, const std::span<const FrameViews> frameViewsArray)
{
    for (auto&& [frameData, frameViews] : std::views::zip(frameDatas_, frameViewsArray))
    {
        frameData.gBufferAlbedo = pipelineResourceManager.RegisterTexture(frameViews.albedo);
        frameData.gBufferNormal = pipelineResourceManager.RegisterTexture(frameViews.normal);
        frameData.gBufferORM = pipelineResourceManager.RegisterTexture(frameViews.orm);
        frameData.gBufferDepth = pipelineResourceManager.RegisterTexture(frameViews.depth);
        frameData.postProcessTarget = pipelineResourceManager.RegisterTexture(frameViews.postProcessTarget);
    }
}

void UFrameContextBuffer::UnregisterTextures(GPipelineResourceManager& pipelineResourceManager) const
{
    for (const auto& frameData : frameDatas_)
    {
        pipelineResourceManager.UnregisterTexture(frameData.gBufferAlbedo);
        pipelineResourceManager.UnregisterTexture(frameData.gBufferNormal);
        pipelineResourceManager.UnregisterTexture(frameData.gBufferORM);
        pipelineResourceManager.UnregisterTexture(frameData.gBufferDepth);
        pipelineResourceManager.UnregisterTexture(frameData.postProcessTarget);
    }
}

void UFrameContextBuffer::UpdateBuffer(const u32 frameIndex
    , const UFrameContextSceneView& sceneView
    , const UFrameContextAddresses& addresses
    , const u32 samplerIndex
)
{    
    const auto viewProj{ sceneView.proj * sceneView.view };

    const Data data{
        .view = sceneView.view,
        .proj = sceneView.proj,
        .invProj = glm::inverse(sceneView.proj),
        .viewProj = viewProj,
        .invViewProj = glm::inverse(viewProj),
        .viewPos = sceneView.viewPos,

        .windowSize = sceneView.windowSize,

        .time = sceneView.time,

        .drawDataBufferAddress = addresses.drawDataBufferAddress,
        .materialBufferAddress = addresses.materialBufferAddress,
        .transformBufferAddress = addresses.transformBufferAddress,
        .parametersBufferAddress = addresses.parametersBufferAddress,

        .directionalLightBufferAddress = addresses.directionalLightBufferAddress,
        .pointLightBufferAddress = addresses.pointLightBufferAddress,
        .directionalLightCount = addresses.directionalLightCount,
        .pointLightCount = addresses.pointLightCount,

        .clusterAABBBufferAddress = addresses.clusterAABBBufferAddress,
        .clusterGridBufferAddress = addresses.clusterGridBufferAddress,
        .lightIndexBufferAddress = addresses.lightIndexBufferAddress,
        .lightCounterBufferAddress = addresses.lightCounterBufferAddress,

        .gBufferAlbedo = frameDatas_[frameIndex].gBufferAlbedo,
        .gBufferNormal = frameDatas_[frameIndex].gBufferNormal,
        .gBufferORM = frameDatas_[frameIndex].gBufferORM,
        .gBufferDepth = frameDatas_[frameIndex].gBufferDepth,
        .gBufferSampler = samplerIndex,

        .postProcessTarget = frameDatas_[frameIndex].postProcessTarget,
        .postProcessSampler = samplerIndex,
    };

    std::memcpy(frameDatas_[frameIndex].dataBuffer.allocationInfo.pMappedData
        , &data
        , sizeof(Data)
    );
}

VkDeviceAddress UFrameContextBuffer::GetAddress(const u32 frameIndex) const
{
    return frameDatas_[frameIndex].dataBuffer.bda;
}

void UFrameContextBuffer::CreateBuffers()
{
    for (auto& frameData : frameDatas_)
    {
        Context.CreateBuffer(frameData.dataBuffer
            , sizeof(Data)
            , VK_BUFFER_USAGE_STORAGE_BUFFER_BIT
            | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT
            , VMA_ALLOCATION_CREATE_MAPPED_BIT
            | VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT
        );
    }
}
