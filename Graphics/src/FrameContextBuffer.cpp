#include "FrameContextBuffer.h"
#include "Camera.h"
#include "GPUBuffers.h"
#include "DrawDataBuffer.h"
#include "LightBuffers.h"
#include "MaterialBuffer.h"
#include "ParametersBuffer.h"
#include "PipelineResourceManager.h"
#include "Renderer.h"
#include "Sampler.h"
#include "TransformBuffer.h"
#include <kotono_common/AssetManager.h>
#include <kotono_platform/Context.h>
#include <kotono_platform/WindowViewport.h>
#include <kotono_timing/Clock.h>

void GFrameContextBuffer::Init()
{
    CreateBuffers();
    RegisterGBufferTextures();
}

void GFrameContextBuffer::Cleanup() const
{
    for (const auto& frameData : frameDatas_)
    {
        vmaDestroyBuffer(Context.GetAllocator(), frameData.dataBuffer.buffer, frameData.dataBuffer.allocation);
    }
}

void GFrameContextBuffer::RegisterGBufferTextures()
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

void GFrameContextBuffer::UnregisterGBufferTextures()
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

void GFrameContextBuffer::UpdateBuffer(const u32 frameIndex)
{
    static UAsset sampler{ SAssetManager<USampler>::Get("${ENGINE_DIRECTORY}/Graphics/assets/samplers/default.kasset") };
    if (!sampler)
    {
        return;
    }
    
    const auto view{ SCamera::GetViewMatrix() };
    const auto proj{ SCamera::GetProjectionMatrix() };
    const auto viewProj{ proj * view };

    const Data data{
        .view = view,
        .proj = proj,
        .invProj = glm::inverse(proj),
        .viewProj = viewProj,
        .invViewProj = glm::inverse(viewProj),
        .viewPos = SCamera::GetPosition(),

        .windowSize = WindowViewport.GetExtent(),

        .time = SClock::Now(),

        .drawDataBufferAddress = DrawDataBuffer.GetAddress(frameIndex),
        .materialBufferAddress = MaterialBuffer.GetAddress(),
        .transformBufferAddress = TransformBuffer.GetAddress(frameIndex),
        .parametersBufferAddress = ParametersBuffer.GetAddress(frameIndex),

        .directionalLightBufferAddress = LightBuffers.GetDirectionalLightAddress(frameIndex),
        .pointLightBufferAddress = LightBuffers.GetPointLightAddress(frameIndex),
        .directionalLightCount = LightBuffers.GetDirectionalLightCount(),
        .pointLightCount = LightBuffers.GetPointLightCount(),

        .clusterAABBBufferAddress = GPUBuffers.GetClusterAABBAddress(),
        .clusterGridBufferAddress = GPUBuffers.GetClusterGridAddress(frameIndex),
        .lightIndexBufferAddress = GPUBuffers.GetLightIndexAddress(frameIndex),
        .lightCounterBufferAddress = GPUBuffers.GetLightCounterAddress(frameIndex),

        .gBufferAlbedo = frameDatas_[frameIndex].gBufferAlbedo,
        .gBufferNormal = frameDatas_[frameIndex].gBufferNormal,
        .gBufferORM = frameDatas_[frameIndex].gBufferORM,
        .gBufferDepth = frameDatas_[frameIndex].gBufferDepth,
        .gBufferSampler = sampler->GetIndex(),

        .postProcessTarget = frameDatas_[frameIndex].postProcessTarget,
        .postProcessSampler = sampler->GetIndex(),
    };

    std::memcpy(frameDatas_[frameIndex].dataBuffer.allocationInfo.pMappedData
        , &data
        , sizeof(Data)
    );
}

VkDeviceAddress GFrameContextBuffer::GetAddress(const u32 frameIndex) const
{
    return frameDatas_[frameIndex].dataBuffer.bda;
}

void GFrameContextBuffer::CreateBuffers()
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
