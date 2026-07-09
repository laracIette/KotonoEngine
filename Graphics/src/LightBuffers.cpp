#include "LightBuffers.h"
#include <kotono_common/AssetManager.h>
#include <kotono_graphics/PipelineResourceManager.h>
#include <kotono_graphics/Renderer.h>
#include <kotono_graphics/Sampler.h>
#include <kotono_platform/Context.h>
#include <kotono_platform/vk_utils.h>

static constexpr u32 MAX_DIRECTIONAL_LIGHTS{ 16 };
static constexpr u32 MAX_POINT_LIGHTS{ 1024 };

void GLightBuffers::Init()
{
    CreateBuffers();
    CreateShadowMapResources();
}

void GLightBuffers::Cleanup() const
{
    for (const auto& frameData : frameDatas_)
    {
        for (const auto& shadowMapTarget : frameData.directionalLightShadowMapTargets)
        {
            vkDestroyImageView(Context.GetDevice(), shadowMapTarget.allocatedImage.imageView, nullptr);
            vmaDestroyImage(Context.GetAllocator(), shadowMapTarget.allocatedImage.image, shadowMapTarget.allocatedImage.allocation);
        }

        vmaDestroyBuffer(Context.GetAllocator(), frameData.directionalLightBuffer.buffer, frameData.directionalLightBuffer.allocation);
        vmaDestroyBuffer(Context.GetAllocator(), frameData.pointLightBuffer.buffer, frameData.pointLightBuffer.allocation);
    }
}

void GLightBuffers::RegisterDirectionalLight(const DirectionalLightData& directionalLight) // todo: add way to update / delete
{
    static UAsset sampler{ SAssetManager<USampler>::Get("${ENGINE_DIRECTORY}/Graphics/assets/samplers/shadow.kasset") };
    if (!sampler)
    {
        return;
    }

    directionalLights_.push_back({
        .direction = directionalLight.direction,
        .color = directionalLight.color,
        .intensity = directionalLight.intensity,
        .castShadow = static_cast<u32>(directionalLight.castShadow),
        .shadowMap = 0, // updated each frame
        .shadowSampler = sampler->GetIndex(),
        .lightViewProj = directionalLight.lightViewProj,
    });
}

void GLightBuffers::RegisterPointLight(const UPointLight& pointLight)
{
    pointLights_.push_back(pointLight);
}

void GLightBuffers::UpdateBuffers(const u32 frameIndex)
{
    for (size i{ 0 }; i < directionalLights_.size(); ++i)
    {
        auto& directionalLight{ directionalLights_[i] };
        auto& shadowMapTarget{ frameDatas_[frameIndex].directionalLightShadowMapTargets[i] };
        directionalLight.shadowMap = shadowMapTarget.textureIndex;
    }

    std::memcpy(frameDatas_[frameIndex].directionalLightBuffer.mapped
        , directionalLights_.data()
        , directionalLights_.size() * sizeof(UDirectionalLight)
    );
    std::memcpy(frameDatas_[frameIndex].pointLightBuffer.mapped
        , pointLights_.data()
        , pointLights_.size() * sizeof(UPointLight)
    );
}

VkDeviceAddress GLightBuffers::GetDirectionalLightAddress(const u32 frameIndex) const
{
    return frameDatas_[frameIndex].directionalLightBuffer.bda;
}

VkDeviceAddress GLightBuffers::GetPointLightAddress(const u32 frameIndex) const
{
    return frameDatas_[frameIndex].pointLightBuffer.bda;
}

u32 GLightBuffers::GetDirectionalLightCount() const
{
    return directionalLights_.size();
}

u32 GLightBuffers::GetPointLightCount() const
{
    return pointLights_.size();
}

void GLightBuffers::CreateBuffers()
{
    for (auto& frameData : frameDatas_)
    {
        CreateBuffer(frameData.directionalLightBuffer, sizeof(UDirectionalLight) * MAX_DIRECTIONAL_LIGHTS);
        CreateBuffer(frameData.pointLightBuffer, sizeof(UPointLight) * MAX_POINT_LIGHTS);
    }
}

void GLightBuffers::CreateBuffer(LightBuffer& lightBuffer, const VkDeviceSize size)
{
    const VkBufferCreateInfo bufInfo{
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .size = size,
        .usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT
            | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT
            | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
    };
    const VmaAllocationCreateInfo allocInfo{
        .flags = VMA_ALLOCATION_CREATE_MAPPED_BIT
                | VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT,
        .usage = VMA_MEMORY_USAGE_AUTO
    };

    VmaAllocationInfo allocationInfo;
    VK_CHECK_THROW(
        vmaCreateBuffer(Context.GetAllocator()
            , &bufInfo
            , &allocInfo
            , &lightBuffer.buffer
            , &lightBuffer.allocation
            , &allocationInfo
        ),
        "failed to create buffer!"
    );
    lightBuffer.mapped = allocationInfo.pMappedData;

    const VkBufferDeviceAddressInfo addrInfo{
        .sType = VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO,
        .buffer = lightBuffer.buffer,
    };
    lightBuffer.bda = vkGetBufferDeviceAddress(Context.GetDevice(), &addrInfo);
}

void GLightBuffers::CreateShadowMapResources()
{
    constexpr u32 SHADOW_MAP_RESOLUTION{ 1024 };

    for (auto& frameData : frameDatas_)
    {
        frameData.directionalLightShadowMapTargets.resize(MAX_DIRECTIONAL_LIGHTS);
        for (auto& shadowMapTarget : frameData.directionalLightShadowMapTargets)
        {
            Context.CreateSampledImageAndImageView(shadowMapTarget.allocatedImage
                , { SHADOW_MAP_RESOLUTION , SHADOW_MAP_RESOLUTION }
                , Renderer.GetDepthFormat()
                , VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT
                , VK_IMAGE_ASPECT_DEPTH_BIT
            );
            shadowMapTarget.textureIndex = PipelineResourceManager.RegisterTexture(shadowMapTarget.allocatedImage.imageView);
        }
    }
}
