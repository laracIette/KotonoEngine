#include "LightBuffers.h"

#include "Barriers.h"
#include "DirectionalLight.h"
#include "PipelineResourceManager.h"
#include "PointLight.h"
#include <kotono_platform/Context.h>
#include <ranges>

static constexpr u32 MAX_DIRECTIONAL_LIGHTS{ 8 };
static constexpr u32 MAX_POINT_LIGHTS{ 1024 };
static constexpr u32 SHADOW_MAP_RESOLUTION{ 2048 }; // todo: make variable

void ULightBuffers::Init(UPipelineResourceManager& pipelineResourceManager)
{
    CreateBuffers();
    CreateShadowMapResources(pipelineResourceManager);
}

void ULightBuffers::Cleanup() const
{
    for (const auto& shadowMapTarget : directionalLightShadowMapTargets_)
    {
        vkDestroyImageView(Context.GetDevice(), shadowMapTarget.allocatedImage.imageView, nullptr);
        vmaDestroyImage(Context.GetAllocator(), shadowMapTarget.allocatedImage.image, shadowMapTarget.allocatedImage.allocation);
    }

    vmaDestroyBuffer(Context.GetAllocator(), directionalLightBuffer_.buffer, directionalLightBuffer_.allocation);
    vmaDestroyBuffer(Context.GetAllocator(), pointLightBuffer_.buffer, pointLightBuffer_.allocation);
}

void ULightBuffers::UpdateBuffers(
      UFrameContextSceneView const& sceneView
    , std::span<UDirectionalLight const> directionalLights
    , std::span<UPointLight const> pointLights
) const
{
    std::memcpy(directionalLightBuffer_.allocationInfo.pMappedData
        , directionalLights.data()
        , directionalLights.size() * sizeof(UDirectionalLight)
    );
    std::memcpy(pointLightBuffer_.allocationInfo.pMappedData
        , pointLights.data()
        , pointLights.size() * sizeof(UPointLight)
    );
}

VkDeviceAddress ULightBuffers::GetDirectionalLightAddress() const
{
    return directionalLightBuffer_.bda;
}

VkDeviceAddress ULightBuffers::GetPointLightAddress() const
{
    return pointLightBuffer_.bda;
}

u32 ULightBuffers::GetDirectionalLightShadowMapTargetIndex(u32 index) const
{
    return directionalLightShadowMapTargets_[index].textureIndex;
}

void ULightBuffers::CmdSetViewportAndScissor(VkCommandBuffer commandBuffer) const
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

void ULightBuffers::CmdBarrierShadowMapsNoneToWrite(VkCommandBuffer commandBuffer) const
{
    auto const images{ directionalLightShadowMapTargets_
        | std::views::transform(&ShadowMapTarget::allocatedImage)
        | std::views::transform(&UAllocatedImage::image)
        | std::ranges::to<std::vector>()
    };

    Barriers::CmdTransitionImages(commandBuffer
        , images
        , VK_PIPELINE_STAGE_2_NONE
        , VK_PIPELINE_STAGE_2_EARLY_FRAGMENT_TESTS_BIT 
        | VK_PIPELINE_STAGE_2_LATE_FRAGMENT_TESTS_BIT
        , VK_ACCESS_2_NONE
        , VK_ACCESS_2_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT
        , VK_IMAGE_LAYOUT_UNDEFINED
        , VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL
        , { VK_IMAGE_ASPECT_DEPTH_BIT, 0, 1, 0, NUM_DIRECTIONAL_CASCADES }
    );
}

void ULightBuffers::CmdBeginRenderingShadowMapTarget(VkCommandBuffer commandBuffer, u32 index) const
{
    const auto& shadowMapTarget{ directionalLightShadowMapTargets_[index] };
    
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

void ULightBuffers::CmdBarrierShadowMapsWriteToShaderRead(VkCommandBuffer commandBuffer) const
{
    auto const images{ directionalLightShadowMapTargets_
        | std::views::transform(&ShadowMapTarget::allocatedImage)
        | std::views::transform(&UAllocatedImage::image)
        | std::ranges::to<std::vector>()
    };

    Barriers::CmdTransitionImages(commandBuffer
        , images
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

void ULightBuffers::CreateBuffers()
{
    Context.CreateBuffer(directionalLightBuffer_
        , sizeof(UDirectionalLight) * MAX_DIRECTIONAL_LIGHTS
        , VK_BUFFER_USAGE_STORAGE_BUFFER_BIT
        | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT
        , VMA_ALLOCATION_CREATE_MAPPED_BIT
        | VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT
    );
    Context.CreateBuffer(pointLightBuffer_
        , sizeof(UPointLight) * MAX_POINT_LIGHTS
        , VK_BUFFER_USAGE_STORAGE_BUFFER_BIT
        | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT
        , VMA_ALLOCATION_CREATE_MAPPED_BIT
        | VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT
    );
}

void ULightBuffers::CreateShadowMapResources(UPipelineResourceManager& pipelineResourceManager)
{
    directionalLightShadowMapTargets_.resize(MAX_DIRECTIONAL_LIGHTS);
    for (auto& shadowMapTarget : directionalLightShadowMapTargets_)
    {
        Context.CreateImageAndImageView(shadowMapTarget.allocatedImage, UAllocatedImageCreateInfo::CreateSampled2D(
              SHADOW_MAP_RESOLUTION
            , SHADOW_MAP_RESOLUTION
            , 1
            , Context.GetDepthFormat()
            , VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT
            , VK_IMAGE_ASPECT_DEPTH_BIT
            , VK_IMAGE_VIEW_TYPE_2D_ARRAY
            , NUM_DIRECTIONAL_CASCADES
        )
        );
        shadowMapTarget.textureIndex = pipelineResourceManager.RegisterTextureArray(shadowMapTarget.allocatedImage.imageView);
    }
}
