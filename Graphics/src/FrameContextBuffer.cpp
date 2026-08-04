#include "FrameContextBuffer.h"
#include "PipelineResourceManager.h"
#include "Renderer.h"
#include <kotono_platform/Context.h>

void UFrameContextBuffer::Init()
{
    CreateBuffers();
    RegisterGBufferTextures();
}

void UFrameContextBuffer::Cleanup() const
{
    for (const auto& frameData : frameDatas_)
    {
        vmaDestroyBuffer(Context.GetAllocator(), frameData.dataBuffer.buffer, frameData.dataBuffer.allocation);
    }
}

void UFrameContextBuffer::RegisterGBufferTextures()
{
    for (u32 i{ 0 }; i < KT_FRAMES_IN_FLIGHT; ++i)
    {
        auto& frameData{ frameDatas_[i] };
        frameData.gBufferAlbedo = PipelineResourceManager.RegisterTexture(Renderer.GetGBufferAlbedoImageView(i));
        frameData.gBufferNormal = PipelineResourceManager.RegisterTexture(Renderer.GetGBufferNormalImageView(i));
        frameData.gBufferORM = PipelineResourceManager.RegisterTexture(Renderer.GetGBufferORMImageView(i));
        frameData.gBufferDepth = PipelineResourceManager.RegisterTexture(Renderer.GetGBufferDepthImageView(i));
        frameData.postProcessTarget = PipelineResourceManager.RegisterTexture(Renderer.GetColorTargetImageView(i));
    }
}

void UFrameContextBuffer::UnregisterGBufferTextures()
{
    for (const auto& frameData : frameDatas_)
    {
        PipelineResourceManager.UnregisterTexture(frameData.gBufferAlbedo);
        PipelineResourceManager.UnregisterTexture(frameData.gBufferNormal);
        PipelineResourceManager.UnregisterTexture(frameData.gBufferORM);
        PipelineResourceManager.UnregisterTexture(frameData.gBufferDepth);
        PipelineResourceManager.UnregisterTexture(frameData.postProcessTarget);
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
