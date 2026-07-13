#include "LightBuffers.h"
#include "Camera.h"
#include "PipelineResourceManager.h"
#include "Renderer.h"
#include "Sampler.h"
#include <kotono_common/AssetManager.h>
#include <kotono_common/log.h>
#include <kotono_platform/Context.h>
#include <kotono_platform/glm_utils.h>
#include <kotono_platform/vk_utils.h>
#include <kotono_platform/WindowViewport.h>
#include <ranges>

static constexpr u32 MAX_DIRECTIONAL_LIGHTS{ 8 };
static constexpr u32 MAX_POINT_LIGHTS{ 1024 };
static constexpr u32 SHADOW_MAP_RESOLUTION{ 4096 }; // todo: make variable

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
        KT_LOG(KT_LOG_COMPILE_TIME_LEVEL, "Graphics", "couldn't load sampler {0}", sampler.Path().ToString());
    }

    directionalLights_.push_back({
        .direction = directionalLight.direction,
        .color = directionalLight.color,
        .intensity = directionalLight.intensity,
        .castShadow = static_cast<u32>(directionalLight.castShadow),
        .shadowMap = {}, // updated each frame
        .shadowSampler = sampler->GetIndex(),
        .lightViewProjs = {}, // updated each frame
        .cascadeSplits = { 5.0f, 15.0f, 50.0f, 200.0f },
    });
}

void GLightBuffers::RegisterPointLight(const UPointLight& pointLight)
{
    pointLights_.push_back(pointLight);
}

void GLightBuffers::UpdateBuffers(const u32 frameIndex)
{
    const std::array<f32, NUM_DIRECTIONAL_CASCADES + 1> cascadeSplits{
       SCamera::GetDepthNear(),
       5.0f,
       15.0f,
       50.0f,
       200.0f,
    };

    constexpr auto makeLightViewProj{ [](const UDirectionalLight& light, const f32 zFar) {
        return get_light_space_matrix(light.direction
            , SCamera::GetViewMatrix()
            , SCamera::GetDepthNear()
            , zFar
            , SCamera::GetFOV()
            , WindowViewport.GetAspectRatio()
        );
    } };

    for (size i{ 0 }; i < directionalLights_.size(); ++i)
    {
        auto& directionalLight{ directionalLights_[i] };
        auto& shadowMapTarget{ frameDatas_[frameIndex].directionalLightShadowMapTargets[i] };
        directionalLight.shadowMap = shadowMapTarget.textureIndex;

        for (u32 i{ 0 }; i < NUM_DIRECTIONAL_CASCADES; ++i)
        {
            directionalLight.lightViewProjs[i] = get_light_space_matrix(
                directionalLight.direction,
                SCamera::GetViewMatrix(),
                cascadeSplits[i],
                cascadeSplits[i + 1],
                SCamera::GetFOV(),
                WindowViewport.GetAspectRatio()
            );
        }
    }

    std::memcpy(frameDatas_[frameIndex].directionalLightBuffer.allocationInfo.pMappedData
        , directionalLights_.data()
        , directionalLights_.size() * sizeof(UDirectionalLight)
    );
    std::memcpy(frameDatas_[frameIndex].pointLightBuffer.allocationInfo.pMappedData
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

void GLightBuffers::CmdSetViewportAndScissor(VkCommandBuffer commandBuffer) const
{
    const VkRect2D scissor{
        .offset = { 0, 0 },
        .extent = { static_cast<i32>(SHADOW_MAP_RESOLUTION), static_cast<i32>(SHADOW_MAP_RESOLUTION) },
    };
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

    const VkViewport viewport{
        .x = 0.0f,
        .y = 0.0f,
        .width = static_cast<f32>(SHADOW_MAP_RESOLUTION),
        .height = static_cast<f32>(SHADOW_MAP_RESOLUTION),
        .minDepth = 0.0f,
        .maxDepth = 1.0f,
    };
    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
}

void GLightBuffers::CmdBarrierShadowMapsNoneToWrite(VkCommandBuffer commandBuffer, const u32 frameIndex) const
{
    const auto images{ frameDatas_[frameIndex].directionalLightShadowMapTargets
        | std::views::transform(&ShadowMapTarget::allocatedImage)
        | std::views::transform(&UAllocatedImage::image)
        | std::ranges::to<std::vector>()
    };

    Renderer.CmdTransitionImages(commandBuffer
        , images.data(), images.size()
        , VK_PIPELINE_STAGE_2_NONE // top of pipe ?
        , VK_PIPELINE_STAGE_2_EARLY_FRAGMENT_TESTS_BIT 
        | VK_PIPELINE_STAGE_2_LATE_FRAGMENT_TESTS_BIT
        , VK_ACCESS_2_NONE
        , VK_ACCESS_2_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT
        , VK_IMAGE_LAYOUT_UNDEFINED
        , VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL
        , { VK_IMAGE_ASPECT_DEPTH_BIT, 0, 1, 0, NUM_DIRECTIONAL_CASCADES }
    );
}

void GLightBuffers::CmdBeginRenderingShadowMapTarget(VkCommandBuffer commandBuffer, const u32 index, const u32 frameIndex) const
{
    const auto& shadowMapTarget{ frameDatas_[frameIndex].directionalLightShadowMapTargets[index] };
    
    const VkRenderingAttachmentInfo depthAttachment{
        .sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,

        .imageView = shadowMapTarget.allocatedImage.imageView,
        .imageLayout = VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL,

        .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
        .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
        .clearValue{
            .depthStencil = { 0.0f, 0 }, // 0.0f instead of 1.0f because of reverse depth
        },
    };
    const VkRenderingInfo renderInfo{
        .sType = VK_STRUCTURE_TYPE_RENDERING_INFO,
        .renderArea{
            .offset = { 0, 0 },
            .extent = { SHADOW_MAP_RESOLUTION, SHADOW_MAP_RESOLUTION },
        },
        .layerCount = 1, // only 1 because multimask
        .viewMask = (1 << NUM_DIRECTIONAL_CASCADES) - 1,
        .colorAttachmentCount = 0,
        .pDepthAttachment = &depthAttachment,
    };
    vkCmdBeginRendering(commandBuffer, &renderInfo);
}

void GLightBuffers::CmdBarrierShadowMapsWriteToShaderRead(VkCommandBuffer commandBuffer, const u32 frameIndex) const
{
    const auto images{ frameDatas_[frameIndex].directionalLightShadowMapTargets
        | std::views::transform(&ShadowMapTarget::allocatedImage)
        | std::views::transform(&UAllocatedImage::image)
        | std::ranges::to<std::vector>()
    };

    Renderer.CmdTransitionImages(commandBuffer
        , images.data(), images.size()
        , VK_PIPELINE_STAGE_2_EARLY_FRAGMENT_TESTS_BIT
        | VK_PIPELINE_STAGE_2_LATE_FRAGMENT_TESTS_BIT
        , VK_PIPELINE_STAGE_2_FRAGMENT_SHADER_BIT
        , VK_ACCESS_2_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT
        , VK_ACCESS_2_SHADER_SAMPLED_READ_BIT
        , VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL
        , VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL
        , { VK_IMAGE_ASPECT_DEPTH_BIT, 0, 1, 0, NUM_DIRECTIONAL_CASCADES }
    );
}

void GLightBuffers::CreateBuffers()
{
    for (auto& frameData : frameDatas_)
    {
        Context.CreateBuffer(frameData.directionalLightBuffer
            , sizeof(UDirectionalLight) * MAX_DIRECTIONAL_LIGHTS
            , VK_BUFFER_USAGE_STORAGE_BUFFER_BIT
            | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT
            , VMA_ALLOCATION_CREATE_MAPPED_BIT
            | VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT
        );
        Context.CreateBuffer(frameData.pointLightBuffer
            , sizeof(UPointLight) * MAX_POINT_LIGHTS
            , VK_BUFFER_USAGE_STORAGE_BUFFER_BIT
            | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT
            , VMA_ALLOCATION_CREATE_MAPPED_BIT
            | VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT
        );
    }
}

void GLightBuffers::CreateShadowMapResources()
{
    for (auto& frameData : frameDatas_)
    {
        frameData.directionalLightShadowMapTargets.resize(MAX_DIRECTIONAL_LIGHTS);
        for (auto& shadowMapTarget : frameData.directionalLightShadowMapTargets)
        {
            Context.CreateSampledImageAndImageView(shadowMapTarget.allocatedImage
                , { SHADOW_MAP_RESOLUTION , SHADOW_MAP_RESOLUTION }
                , NUM_DIRECTIONAL_CASCADES
                , Renderer.GetDepthFormat()
                , VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT
                , VK_IMAGE_VIEW_TYPE_2D_ARRAY
                , VK_IMAGE_ASPECT_DEPTH_BIT
            );
            shadowMapTarget.textureIndex = PipelineResourceManager.RegisterTextureArray(shadowMapTarget.allocatedImage.imageView);
        }
    }
}
