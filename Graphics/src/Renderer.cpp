#include "Renderer.h"
#include "DrawCall.h"
#include "DrawDataBuffer.h"
#include "FrameContextBuffer.h"
#include "GPUBuffers.h"
#include "LightBuffers.h"
#include "IndexBuffer.h"
#include "MaterialBuffer.h"
#include "ParametersBuffer.h"
#include "PipelineResourceManager.h"
#include "TransformBuffer.h"
#include "Shader.h"
#include "SwapChain.h"
#include <kotono_platform/Context.h>
#include <kotono_platform/Window.h>
#include <kotono_platform/WindowViewport.h>
#include <kotono_common/AssetManager.h>
#include <kotono_common/log.h>
#include <kotono_platform/vk_utils.h>
#include <map>
#include <ranges>
#include <unordered_map>

void GRenderer::Init()
{
	SwapChain.Init();

	CreateImageResources();
	CreateCommandPools();
	CreateCommandBuffers();
	CreateSyncObjects();

	PipelineResourceManager.Init();
	FrameContextBuffer.Init();
	DrawDataBuffer.Init();
	MaterialBuffer.Init();
	TransformBuffer.Init();
	ParametersBuffer.Init();
	LightBuffers.Init();
	GPUBuffers.Init();
	IndexBuffer.Init();
}

void GRenderer::Cleanup()
{
	KT_LOG(ELogImportanceLevel::High, "Graphics", "cleaning up renderer");

	JoinThread(renderThread_);
	JoinThread(rhiThread_); 

	IndexBuffer.Cleanup();
	GPUBuffers.Cleanup();
	LightBuffers.Cleanup();
	ParametersBuffer.Cleanup();
	TransformBuffer.Cleanup();
	MaterialBuffer.Cleanup();
	DrawDataBuffer.Cleanup();
	FrameContextBuffer.Cleanup();
	PipelineResourceManager.Cleanup();

	CleanupImageResources();
	
	SwapChain.Cleanup();

	for (const auto& frameData : frameDatas_)
	{
		vkDestroySemaphore(Context.GetDevice(), frameData.renderFinishedSemaphore, nullptr);
		vkDestroySemaphore(Context.GetDevice(), frameData.imageAvailableSemaphore, nullptr);
		vkDestroyFence(Context.GetDevice(), frameData.inFlightFence, nullptr);
		vkDestroyCommandPool(Context.GetDevice(), frameData.commandPool, nullptr);
	}

	KT_LOG(ELogImportanceLevel::High, "Graphics", "cleaned up renderer");
}

static constexpr bool IS_MULTI_THREADED{ false };
void GRenderer::DrawFrame()
{
	const u32 frameIndex{ GetGameThreadFrame() };

	FrameContextBuffer.UpdateBuffer(frameIndex);
	DrawDataBuffer.UpdateBuffer(frameIndex);
	TransformBuffer.UpdateBuffer(frameIndex);
	ParametersBuffer.UpdateBuffer(frameIndex);
	LightBuffers.UpdateBuffers(frameIndex);

	if constexpr (IS_MULTI_THREADED)
	{
		if (frameCount_ >= 1)
		{
			JoinThread(renderThread_);
			const u32 renderThreadFrame{ GetRenderThreadFrame() };
			renderThread_ = std::thread(&GRenderer::RecordCommandBuffer, this, renderThreadFrame);
		}

		if (frameCount_ >= 2)
		{
			KT_LOG(ELogImportanceLevel::High, "Graphics", "frame {0} rendered", frameCount_);

			JoinThread(rhiThread_);
			Context.ExecuteSingleTimeCommands();
			const u32 renderRHIFrame{ GetRHIThreadFrame() };
			rhiThread_ = std::thread(&GRenderer::SubmitCommandBuffer, this, renderRHIFrame);
		}
	}
	else
	{
		if (!TryAcquireNextImage(frameIndex))
		{
			KT_LOG(ELogImportanceLevel::High, "Graphics", "frame {0} skipped", frameCount_);
			return;
		}

		RecordCommandBuffer(frameIndex);
		Context.ExecuteSingleTimeCommands();
		SubmitCommandBuffer(frameIndex);
	}

	frameCount_++;
}

VkFormat GRenderer::GetDepthFormat() const
{
	return depthFormat_;
}

void GRenderer::RegisterOpaqueDrawCall(UDrawCall* drawCall)
{
	if (!drawCall)
	{
		return;
	}

	opaqueDrawCalls_.Add(drawCall);
	drawCall->poolIndex = opaqueDrawCalls_.LastIndex();
}

void GRenderer::RegisterInterfaceDrawCall(UDrawCall* drawCall)
{
	if (!drawCall)
	{
		return;
	}

	interfaceDrawCalls_.Add(drawCall);
	drawCall->poolIndex = interfaceDrawCalls_.LastIndex();
}

void GRenderer::UnregisterOpaqueDrawCall(UDrawCall* drawCall)
{
	if (!drawCall)
	{
		return;
	}

	const auto index{ drawCall->poolIndex };
	if (opaqueDrawCalls_.RemoveAt(index) == EPoolRemoveResult::ItemSwappedAndRemoved)
	{
		opaqueDrawCalls_[index]->poolIndex = index;
	}
}

void GRenderer::UnregisterInterfaceDrawCall(UDrawCall* drawCall)
{
	if (!drawCall)
	{
		return;
	}

	const auto index{ drawCall->poolIndex };
	if (interfaceDrawCalls_.RemoveAt(index) == EPoolRemoveResult::ItemSwappedAndRemoved)
	{
		interfaceDrawCalls_[index]->poolIndex = index;
	}
}

VkImageView GRenderer::GetGBufferAlbedoImageView(const u32 frameIndex) const
{
	return frameDatas_[frameIndex].albedoTarget.imageView;
}

VkImageView GRenderer::GetGBufferNormalImageView(const u32 frameIndex) const
{
	return frameDatas_[frameIndex].normalTarget.imageView;
}

VkImageView GRenderer::GetGBufferORMImageView(const u32 frameIndex) const
{
	return frameDatas_[frameIndex].ormTarget.imageView;
}

VkImageView GRenderer::GetGBufferDepthImageView(const u32 frameIndex) const
{
	return frameDatas_[frameIndex].depthTarget.imageView;
}

VkImageView GRenderer::GetColorTargetImageView(const u32 frameIndex) const
{
	return frameDatas_[frameIndex].colorTarget.imageView;
}

void GRenderer::CmdTransitionImages(VkCommandBuffer commandBuffer
	, const VkImage* image
	, const u32 count
	, const VkPipelineStageFlags2 srcStage
	, const VkPipelineStageFlags2 dstStage
	, const VkAccessFlags2 srcAccess
	, const VkAccessFlags2 dstAccess
	, const VkImageLayout oldLayout
	, const VkImageLayout newLayout
	, const VkImageSubresourceRange subresourceRange) const
{
	std::vector<VkImageMemoryBarrier2> barriers{};
	barriers.reserve(count);
	for (u32 i{ 0 }; i < count; ++i)
	{
		barriers.push_back({
			.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,
			.srcStageMask = srcStage,
			.srcAccessMask = srcAccess,
			.dstStageMask = dstStage,
			.dstAccessMask = dstAccess,
			.oldLayout = oldLayout,
			.newLayout = newLayout,
			.image = image[i],
			.subresourceRange = subresourceRange
		});
	}
	const VkDependencyInfo dependencyInfo{
		.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
		.imageMemoryBarrierCount = static_cast<u32>(barriers.size()),
		.pImageMemoryBarriers = barriers.data(),
	};
	vkCmdPipelineBarrier2(commandBuffer, &dependencyInfo);
}

void GRenderer::CmdTransitionCompute(VkCommandBuffer commandBuffer
	, const VkPipelineStageFlags2 srcStage
	, const VkPipelineStageFlags2 dstStage
	, const VkAccessFlags2 srcAccess
	, const VkAccessFlags2 dstAccess) const
{
	const VkMemoryBarrier2 barrier{
		.sType = VK_STRUCTURE_TYPE_MEMORY_BARRIER_2,
		.srcStageMask = srcStage,
		.srcAccessMask = srcAccess,
		.dstStageMask = dstStage,
		.dstAccessMask = dstAccess,
	};
	const VkDependencyInfo dependencyInfo{
		.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
		.memoryBarrierCount = 1,
		.pMemoryBarriers = &barrier,
	};
	vkCmdPipelineBarrier2(commandBuffer, &dependencyInfo);
}

void GRenderer::CreateImageResources()
{
	depthFormat_ = FindDepthFormat();
	const auto extent{ SwapChain.GetExtent() };

	for (auto& frameData : frameDatas_)
	{
		Context.CreateSampledImageAndImageView(frameData.colorTarget,	extent, 1, VK_FORMAT_R16G16B16A16_SFLOAT,	VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,			VK_IMAGE_VIEW_TYPE_2D, VK_IMAGE_ASPECT_COLOR_BIT);
		Context.CreateSampledImageAndImageView(frameData.albedoTarget,	extent, 1, VK_FORMAT_R8G8B8A8_SRGB,			VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,			VK_IMAGE_VIEW_TYPE_2D, VK_IMAGE_ASPECT_COLOR_BIT);
		Context.CreateSampledImageAndImageView(frameData.normalTarget,	extent, 1, VK_FORMAT_R16G16B16A16_SFLOAT,	VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,			VK_IMAGE_VIEW_TYPE_2D, VK_IMAGE_ASPECT_COLOR_BIT);
		Context.CreateSampledImageAndImageView(frameData.ormTarget,		extent, 1, VK_FORMAT_R8G8B8A8_UNORM,		VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,			VK_IMAGE_VIEW_TYPE_2D, VK_IMAGE_ASPECT_COLOR_BIT);
		Context.CreateSampledImageAndImageView(frameData.depthTarget,	extent, 1, depthFormat_,					VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,	VK_IMAGE_VIEW_TYPE_2D, VK_IMAGE_ASPECT_DEPTH_BIT);
	}
}

void GRenderer::CleanupImageResources() const
{
	for (const auto& frameData : frameDatas_)
	{
		vkDestroyImageView(Context.GetDevice(), frameData.colorTarget.imageView, nullptr);
		vmaDestroyImage(Context.GetAllocator(), frameData.colorTarget.image, frameData.colorTarget.allocation);

		vkDestroyImageView(Context.GetDevice(), frameData.albedoTarget.imageView, nullptr);
		vmaDestroyImage(Context.GetAllocator(), frameData.albedoTarget.image, frameData.albedoTarget.allocation);
		vkDestroyImageView(Context.GetDevice(), frameData.normalTarget.imageView, nullptr);
		vmaDestroyImage(Context.GetAllocator(), frameData.normalTarget.image, frameData.normalTarget.allocation);
		vkDestroyImageView(Context.GetDevice(), frameData.ormTarget.imageView, nullptr);
		vmaDestroyImage(Context.GetAllocator(), frameData.ormTarget.image, frameData.ormTarget.allocation);

		vkDestroyImageView(Context.GetDevice(), frameData.depthTarget.imageView, nullptr);
		vmaDestroyImage(Context.GetAllocator(), frameData.depthTarget.image, frameData.depthTarget.allocation);
	}
}

VkFormat GRenderer::FindDepthFormat() const
{
	constexpr std::array formats{ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT };
	return Context.FindSupportedFormat(formats, VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
}

bool GRenderer::TryAcquireNextImage(const u32 frameIndex)
{
	// Wait for current frame to be rendered
	VK_CHECK_THROW(
		vkWaitForFences(Context.GetDevice(), 1, &frameDatas_[frameIndex].inFlightFence, VK_TRUE, UINT64_MAX),
		"failed to wait for fences!"
	);

	// Set image index for current frame
	const VkResult result{ vkAcquireNextImageKHR(Context.GetDevice(), SwapChain.GetSwapChain(), UINT64_MAX, frameDatas_[frameIndex].imageAvailableSemaphore, VK_NULL_HANDLE, &frameDatas_[frameIndex].imageIndex)};
	if (result == VK_ERROR_OUT_OF_DATE_KHR)
	{
		RecreateFrame();
		return false;
	}
	else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
	{
		throw std::runtime_error("failed to acquire swap chain image!");
	}

	vkResetFences(Context.GetDevice(), 1, &frameDatas_[frameIndex].inFlightFence);

	return true;
}

void GRenderer::CreateCommandPools()
{
	for (size i{ 0 }; i < KT_FRAMES_IN_FLIGHT; ++i)
	{
		CreateCommandPool(static_cast<u32>(i));
	}
}

void GRenderer::CreateCommandPool(const u32 frameIndex)
{
	const KtQueueFamilyIndices queueFamilyIndices{ Context.FindQueueFamilies(Context.GetPhysicalDevice()) };

	const VkCommandPoolCreateInfo poolInfo{
		.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
		.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
		.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value(),
	};
	VK_CHECK_THROW(
		vkCreateCommandPool(Context.GetDevice(), &poolInfo, nullptr, &frameDatas_[frameIndex].commandPool),
		"failed to create command pool!"
	);
}

void GRenderer::CreateCommandBuffers()
{
	for (size i{ 0 }; i < KT_FRAMES_IN_FLIGHT; ++i)
	{
		CreateCommandBuffer(static_cast<u32>(i));
	}
}

void GRenderer::CreateCommandBuffer(const u32 frameIndex)
{
	const VkCommandBufferAllocateInfo allocInfo{
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
		.commandPool = frameDatas_[frameIndex].commandPool,
		.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
		.commandBufferCount = 1,
	};
	VK_CHECK_THROW(
		vkAllocateCommandBuffers(Context.GetDevice(), &allocInfo, &frameDatas_[frameIndex].commandBuffer),
		"failed to allocate command buffers!"
	);
}

void GRenderer::CreateSyncObjects()
{
	const VkSemaphoreCreateInfo semaphoreInfo{
		.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
	};
	const VkFenceCreateInfo fenceInfo{
		.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
		.flags = VK_FENCE_CREATE_SIGNALED_BIT,
	};
	for (auto& frameData : frameDatas_)
	{
		if (vkCreateSemaphore(Context.GetDevice(), &semaphoreInfo, nullptr, &frameData.imageAvailableSemaphore) != VK_SUCCESS ||
			vkCreateSemaphore(Context.GetDevice(), &semaphoreInfo, nullptr, &frameData.renderFinishedSemaphore) != VK_SUCCESS ||
			vkCreateFence(Context.GetDevice(), &fenceInfo, nullptr, &frameData.inFlightFence) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create synchronization objects for a frame!");
		}
	}
}

void GRenderer::RecordCommandBuffer(const u32 frameIndex)
{
	VkCommandBuffer commandBuffer{ frameDatas_[frameIndex].commandBuffer };
	vkResetCommandBuffer(commandBuffer, 0);

	BeginCommandBuffer(commandBuffer);

	PipelineResourceManager.CmdBindDescriptorSet(commandBuffer);

	static bool isClusterAABBDirty{ true };
	if (isClusterAABBDirty)
	{
		isClusterAABBDirty = false;
		CmdUpdateClusterAABB(commandBuffer, frameIndex);
	}

	// Light binning 
	// - Wait for fill buffer command to be executable
	CmdBarrierComputeFragmentReadToClearWrite(commandBuffer);
	// - Atomic reset light counter to 0
	CmdResetLightCounter(commandBuffer, frameIndex);
	// - Make light counter accessible and light binning writable
	CmdBarrierComputeClearWriteToReadWrite(commandBuffer);
	// - Compute light binning
	CmdDispatchLightBinning(commandBuffer, frameIndex);
	// - Make light binning accessible to fragment
	CmdBarrierComputeWriteToFragmentRead(commandBuffer);


	// Shadow-maps
	// - Set rendering area to fit shadow-maps
	LightBuffers.CmdSetViewportAndScissor(commandBuffer);
	// - Make shadow maps writable
	LightBuffers.CmdBarrierShadowMapsNoneToWrite(commandBuffer, frameIndex);
	// - Generate shadow-maps
	CmdDrawFrameShadowMaps(commandBuffer, frameIndex);

	// Reset the rendering area to full screen
	WindowViewport.CmdUse(commandBuffer);

	// Depth pre-pass
	// - Make depth writable
	CmdBarrierDepthNoneToWrite(commandBuffer, frameIndex);
	// - Write depth
	CmdBeginRenderingDepthPrePass(commandBuffer, frameIndex);
	CmdDrawFrameDepthPrePass(commandBuffer, frameIndex);
	CmdEndRendering(commandBuffer);

	// G-Buffer
	// - Make depth readable
	CmdBarrierDepthWriteToRead(commandBuffer, frameIndex);
	// - Make G-Buffer writable
	CmdBarrierGBufferNoneToWrite(commandBuffer, frameIndex);
	// - Write G-Buffer
	CmdBeginRenderingGBuffer(commandBuffer, frameIndex);
	CmdDrawFrameGBuffer(commandBuffer, frameIndex);
	CmdEndRendering(commandBuffer);
	// - Make G-Buffer readable
	CmdBarrierGBufferWriteToRead(commandBuffer, frameIndex);

	// Deferred lighting
	// - Make shadow-maps shader-readable
	LightBuffers.CmdBarrierShadowMapsWriteToShaderRead(commandBuffer, frameIndex);
	// - Make depth shader-readable (reconstruct world pos)
	CmdBarrierDepthReadToShaderRead(commandBuffer, frameIndex);
	// - Make color target writable
	CmdBarrierColorNoneToWrite(commandBuffer, frameIndex);
	// - Write color target
	CmdBeginRenderingDeferredLighting(commandBuffer, frameIndex);
	CmdDrawFrameDeferredLighting(commandBuffer, frameIndex);
	CmdEndRendering(commandBuffer);
	// - Make color target readable
	CmdBarrierColorWriteToRead(commandBuffer, frameIndex);

	// Post process
	// - Make swapchain image writable
	CmdBarrierSwapchainNoneToWrite(commandBuffer, frameIndex);
	// - Write swapchain image
	CmdBeginRenderingPostProcess(commandBuffer, frameIndex);
	CmdDrawFramePostProcess(commandBuffer, frameIndex);
	CmdEndRendering(commandBuffer);

	// Interface
	// - Write swapchain image
	CmdBeginRenderingInterface(commandBuffer, frameIndex);
	CmdDrawFrameInterface(commandBuffer, frameIndex);
	CmdEndRendering(commandBuffer);
	// - Make swapchain image presentable
	CmdBarrierSwapchainWriteToPresent(commandBuffer, frameIndex);

	EndCommandBuffer(commandBuffer);
}

void GRenderer::BeginCommandBuffer(VkCommandBuffer commandBuffer)
{
	const VkCommandBufferBeginInfo beginInfo{
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
	};
	VK_CHECK_THROW(
		vkBeginCommandBuffer(commandBuffer, &beginInfo),
		"failed to begin recording command buffer!"
	);
}

void GRenderer::CmdUpdateClusterAABB(VkCommandBuffer commandBuffer, const u32 frameIndex) const
{
	if (UAsset shader{ SAssetManager<UShader>::Get("${ENGINE_DIRECTORY}/Graphics/assets/shaders/clusterAABB.kasset") })
	{
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, shader->GetPipeline());
		CmdPushConstants(commandBuffer, nullptr, frameIndex);
		vkCmdDispatch(commandBuffer, 16, 9, 24);
	}
}

void GRenderer::CmdBarrierComputeFragmentReadToClearWrite(VkCommandBuffer commandBuffer) const
{
	CmdTransitionCompute(commandBuffer
		, VK_PIPELINE_STAGE_2_FRAGMENT_SHADER_BIT
		, VK_PIPELINE_STAGE_2_CLEAR_BIT
		, VK_ACCESS_2_SHADER_READ_BIT
		, VK_ACCESS_2_TRANSFER_WRITE_BIT
	);
}

void GRenderer::CmdResetLightCounter(VkCommandBuffer commandBuffer, const u32 frameIndex) const
{
	vkCmdFillBuffer(commandBuffer
		, GPUBuffers.GetLightCounterBuffer(frameIndex)
		, 0
		, sizeof(u32)
		, 0
	);
}

void GRenderer::CmdBarrierComputeClearWriteToReadWrite(VkCommandBuffer commandBuffer) const
{
	CmdTransitionCompute(commandBuffer
		, VK_PIPELINE_STAGE_2_TRANSFER_BIT
		| VK_PIPELINE_STAGE_2_CLEAR_BIT
		| VK_PIPELINE_STAGE_2_COMPUTE_SHADER_BIT
		, VK_PIPELINE_STAGE_2_COMPUTE_SHADER_BIT
		, VK_ACCESS_2_TRANSFER_WRITE_BIT
		| VK_ACCESS_2_SHADER_WRITE_BIT
		, VK_ACCESS_2_SHADER_READ_BIT
		| VK_ACCESS_2_SHADER_WRITE_BIT
	);
}

void GRenderer::CmdDispatchLightBinning(VkCommandBuffer commandBuffer, const u32 frameIndex) const
{
	if (UAsset binningShader{ SAssetManager<UShader>::Get("${ENGINE_DIRECTORY}/Graphics/assets/shaders/lightBinning.kasset") })
	{
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, binningShader->GetPipeline());
		CmdPushConstants(commandBuffer, nullptr, frameIndex);
		vkCmdDispatch(commandBuffer, 16, 9, 24);
	}
}

void GRenderer::CmdBarrierComputeWriteToFragmentRead(VkCommandBuffer commandBuffer) const
{
	CmdTransitionCompute(commandBuffer
		, VK_PIPELINE_STAGE_2_COMPUTE_SHADER_BIT
		, VK_PIPELINE_STAGE_2_FRAGMENT_SHADER_BIT
		, VK_ACCESS_2_SHADER_WRITE_BIT
		, VK_ACCESS_2_SHADER_READ_BIT
	);
}

void GRenderer::CmdDrawFrameShadowMaps(VkCommandBuffer commandBuffer, const u32 frameIndex) const
{
	static UAsset shader{ SAssetManager<UShader>::Get("${ENGINE_DIRECTORY}/Graphics/assets/shaders/shadowPrePass.kasset") };
	if (!shader)
	{
		KT_LOG(KT_LOG_COMPILE_TIME_LEVEL, "Graphics", "couldn't load shader {0}", shader.Path().ToString());
	}

	IndexBuffer.CmdBind(commandBuffer);

	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, shader->GetPipeline());

	for (u32 i{ 0 }; i < LightBuffers.GetDirectionalLightCount(); ++i)
	{
		LightBuffers.CmdBeginRenderingShadowMapTarget(commandBuffer, i, frameIndex);
		// todo: add cast shadow check
		for (const auto* drawCall : opaqueDrawCalls_)
		{
			const UPushConstants pc{
				.frameContextBufferAddress = FrameContextBuffer.GetAddress(frameIndex),
				.drawIndex = drawCall->index,
				.directionalIndex = i,
			};

			vkCmdPushConstants(commandBuffer
				, PipelineResourceManager.GetPipelineLayout()
				, VK_SHADER_STAGE_ALL
				, 0
				, sizeof(UPushConstants)
				, &pc
			);

			CmdDraw(commandBuffer, drawCall);
		}

		CmdEndRendering(commandBuffer);
	}
}

void GRenderer::CmdBarrierDepthNoneToWrite(VkCommandBuffer commandBuffer, const u32 frameIndex) const
{
	CmdTransitionImages(commandBuffer
		, &frameDatas_[frameIndex].depthTarget.image, 1
		, VK_PIPELINE_STAGE_2_TOP_OF_PIPE_BIT
		, VK_PIPELINE_STAGE_2_EARLY_FRAGMENT_TESTS_BIT
		| VK_PIPELINE_STAGE_2_LATE_FRAGMENT_TESTS_BIT
		, VK_ACCESS_2_NONE
		, VK_ACCESS_2_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT
		, VK_IMAGE_LAYOUT_UNDEFINED
		, VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL
		, { VK_IMAGE_ASPECT_DEPTH_BIT, 0, 1, 0, 1 }
	);
}

void GRenderer::CmdBeginRenderingDepthPrePass(VkCommandBuffer commandBuffer, const u32 frameIndex) const
{
	const VkRenderingAttachmentInfo depthAttachment{
		.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,

		.imageView = frameDatas_[frameIndex].depthTarget.imageView,
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
			.extent = SwapChain.GetExtent()
		},
		.layerCount = 1,
		.colorAttachmentCount = 0,
		.pDepthAttachment = &depthAttachment,
	};
	vkCmdBeginRendering(commandBuffer, &renderInfo);
}

void GRenderer::CmdDrawFrameDepthPrePass(VkCommandBuffer commandBuffer, const u32 frameIndex) const
{
	if (UAsset shader{ SAssetManager<UShader>::Get("${ENGINE_DIRECTORY}/Graphics/assets/shaders/depthPrePass.kasset") })
	{
		IndexBuffer.CmdBind(commandBuffer);

		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, shader->GetPipeline());

		for (const auto* drawCall : opaqueDrawCalls_)
		{
			CmdPushConstants(commandBuffer, drawCall, frameIndex);
			CmdDraw(commandBuffer, drawCall);
		}
	}	
}

void GRenderer::CmdBarrierDepthWriteToRead(VkCommandBuffer commandBuffer, const u32 frameIndex) const
{
	CmdTransitionImages(commandBuffer
		, &frameDatas_[frameIndex].depthTarget.image, 1
		, VK_PIPELINE_STAGE_2_EARLY_FRAGMENT_TESTS_BIT
		| VK_PIPELINE_STAGE_2_LATE_FRAGMENT_TESTS_BIT
		, VK_PIPELINE_STAGE_2_EARLY_FRAGMENT_TESTS_BIT
		| VK_PIPELINE_STAGE_2_LATE_FRAGMENT_TESTS_BIT
		, VK_ACCESS_2_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT
		, VK_ACCESS_2_DEPTH_STENCIL_ATTACHMENT_READ_BIT
		, VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL
		, VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL
		, { VK_IMAGE_ASPECT_DEPTH_BIT, 0, 1, 0, 1 }
	);
}

void GRenderer::CmdBarrierGBufferNoneToWrite(VkCommandBuffer commandBuffer, const u32 frameIndex) const
{
	const std::array images{ 
		frameDatas_[frameIndex].albedoTarget.image,
		frameDatas_[frameIndex].normalTarget.image,
		frameDatas_[frameIndex].ormTarget.image,
	};
	CmdTransitionImages(commandBuffer
		, images.data(), 3
		, VK_PIPELINE_STAGE_2_TOP_OF_PIPE_BIT
		, VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT
		, VK_ACCESS_2_NONE
		, VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT
		, VK_IMAGE_LAYOUT_UNDEFINED
		, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
		, { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 }
	);
}

void GRenderer::CmdBeginRenderingGBuffer(VkCommandBuffer commandBuffer, const u32 frameIndex) const
{
	const auto makeRenderingAttachmentInfo{ [](VkImageView imageView) {
		return VkRenderingAttachmentInfo{
			.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
			.imageView = imageView,
			.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,

			.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
			.storeOp = VK_ATTACHMENT_STORE_OP_STORE,
			.clearValue{
				.color = { 0.0f, 0.0f, 0.0f, 1.0f },
			},
		};
	} };

	const std::array colorAttachments{
		makeRenderingAttachmentInfo(frameDatas_[frameIndex].albedoTarget.imageView),
		makeRenderingAttachmentInfo(frameDatas_[frameIndex].normalTarget.imageView),
		makeRenderingAttachmentInfo(frameDatas_[frameIndex].ormTarget.imageView),
	};

	const VkRenderingAttachmentInfo depthAttachment{
		.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,

		.imageView = frameDatas_[frameIndex].depthTarget.imageView,
		.imageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL,

		.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD,
		.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
		.clearValue{
			.depthStencil = { 0.0f, 0 }, // 0.0f instead of 1.0f because of reverse depth
		},
	};

	const VkRenderingInfo renderingInfo{
		.sType = VK_STRUCTURE_TYPE_RENDERING_INFO,
		.renderArea{
			.offset = { 0, 0 },
			.extent = SwapChain.GetExtent()
		},
		.layerCount = 1,
		.colorAttachmentCount = static_cast<u32>(colorAttachments.size()),
		.pColorAttachments = colorAttachments.data(),
		.pDepthAttachment = &depthAttachment,
	};
	
	vkCmdBeginRendering(commandBuffer, &renderingInfo);
}

void GRenderer::CmdDrawFrameGBuffer(VkCommandBuffer commandBuffer, const u32 frameIndex) const
{
	std::unordered_map<VkPipeline, std::vector<const UDrawCall*>> pipelineDrawCalls{};

	for (const auto* drawCall : opaqueDrawCalls_)
	{
		pipelineDrawCalls[drawCall->pipeline].push_back(drawCall);
	}

	IndexBuffer.CmdBind(commandBuffer);

	for (const auto& [pipeline, drawCalls] : pipelineDrawCalls)
	{
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);

		for (const auto* drawCall : drawCalls)
		{
			CmdPushConstants(commandBuffer, drawCall, frameIndex);
			CmdDraw(commandBuffer, drawCall);
		}
	}
}

void GRenderer::CmdBarrierGBufferWriteToRead(VkCommandBuffer commandBuffer, const u32 frameIndex) const
{
	const std::array images{
		frameDatas_[frameIndex].albedoTarget.image,
		frameDatas_[frameIndex].normalTarget.image,
		frameDatas_[frameIndex].ormTarget.image,
	};
	CmdTransitionImages(commandBuffer
		, images.data(), 3
		, VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT
		, VK_PIPELINE_STAGE_2_FRAGMENT_SHADER_BIT
		, VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT
		, VK_ACCESS_2_SHADER_SAMPLED_READ_BIT
		, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
		, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
		, { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 }
	);
}

void GRenderer::CmdBarrierDepthReadToShaderRead(VkCommandBuffer commandBuffer, const u32 frameIndex) const
{
	CmdTransitionImages(commandBuffer
		, &frameDatas_[frameIndex].depthTarget.image, 1
		, VK_PIPELINE_STAGE_2_EARLY_FRAGMENT_TESTS_BIT 
		| VK_PIPELINE_STAGE_2_LATE_FRAGMENT_TESTS_BIT
		, VK_PIPELINE_STAGE_2_FRAGMENT_SHADER_BIT
		, VK_ACCESS_2_DEPTH_STENCIL_ATTACHMENT_READ_BIT
		, VK_ACCESS_2_SHADER_SAMPLED_READ_BIT
		, VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL
		, VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL
		, { VK_IMAGE_ASPECT_DEPTH_BIT, 0, 1, 0, 1 }
	);
}

void GRenderer::CmdBarrierColorNoneToWrite(VkCommandBuffer commandBuffer, const u32 frameIndex) const
{
	CmdTransitionImages(commandBuffer
		, &frameDatas_[frameIndex].colorTarget.image, 1
		, VK_PIPELINE_STAGE_2_TOP_OF_PIPE_BIT
		, VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT
		, VK_ACCESS_2_NONE
		, VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT
		, VK_IMAGE_LAYOUT_UNDEFINED
		, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
		, { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 }
	);
}

void GRenderer::CmdBeginRenderingDeferredLighting(VkCommandBuffer commandBuffer, const u32 frameIndex) const
{
	const VkRenderingAttachmentInfo colorAttachment{
		.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
		.imageView = frameDatas_[frameIndex].colorTarget.imageView,
		.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,

		.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
		.storeOp = VK_ATTACHMENT_STORE_OP_STORE,
	};
	const VkRenderingInfo renderInfo{
		.sType = VK_STRUCTURE_TYPE_RENDERING_INFO,
		.renderArea{
			.offset = { 0, 0 },
			.extent = SwapChain.GetExtent()
		},
		.layerCount = 1,
		.colorAttachmentCount = 1,
		.pColorAttachments = &colorAttachment,
		.pDepthAttachment = VK_NULL_HANDLE,
	};
	vkCmdBeginRendering(commandBuffer, &renderInfo);
}

void GRenderer::CmdDrawFrameDeferredLighting(VkCommandBuffer commandBuffer, const u32 frameIndex) const
{
	if (UAsset shader{ SAssetManager<UShader>::Get("${ENGINE_DIRECTORY}/Graphics/assets/shaders/deferredLighting.kasset") })
	{
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, shader->GetPipeline());
		CmdPushConstants(commandBuffer, nullptr, frameIndex);
		vkCmdDraw(commandBuffer, 3, 1, 0, 0);
	}
}

void GRenderer::CmdBarrierColorWriteToRead(VkCommandBuffer commandBuffer, const u32 frameIndex) const
{
	CmdTransitionImages(commandBuffer
		, &frameDatas_[frameIndex].colorTarget.image, 1
		, VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT
		, VK_PIPELINE_STAGE_2_FRAGMENT_SHADER_BIT
		, VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT
		, VK_ACCESS_2_SHADER_SAMPLED_READ_BIT
		, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
		, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
		, { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 }
	);
}

void GRenderer::CmdBarrierSwapchainNoneToWrite(VkCommandBuffer commandBuffer, const u32 frameIndex) const
{
	CmdTransitionImages(commandBuffer
		, &SwapChain.GetData(frameDatas_[frameIndex].imageIndex).image, 1
		, VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT
		| VK_PIPELINE_STAGE_2_FRAGMENT_SHADER_BIT
		| VK_PIPELINE_STAGE_2_LATE_FRAGMENT_TESTS_BIT
		, VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT
		, VK_ACCESS_2_NONE
		, VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT
		, VK_IMAGE_LAYOUT_UNDEFINED
		, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
		, { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 }
	);
}

void GRenderer::CmdBeginRenderingPostProcess(VkCommandBuffer commandBuffer, const u32 frameIndex) const
{
	const VkRenderingAttachmentInfo swapchainAttachment{
		.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
		.imageView = SwapChain.GetData(frameDatas_[frameIndex].imageIndex).imageView,
		.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,

		.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
		.storeOp = VK_ATTACHMENT_STORE_OP_STORE,
	};

	const VkRenderingInfo swapchainRenderingInfo{
		.sType = VK_STRUCTURE_TYPE_RENDERING_INFO,
		.renderArea{
			.offset = { 0, 0 },
			.extent = SwapChain.GetExtent()
		},
		.layerCount = 1,
		.colorAttachmentCount = 1,
		.pColorAttachments = &swapchainAttachment,
	};

	vkCmdBeginRendering(commandBuffer, &swapchainRenderingInfo);
}

void GRenderer::CmdDrawFramePostProcess(VkCommandBuffer commandBuffer, const u32 frameIndex) const
{
	if (UAsset shader{ SAssetManager<UShader>::Get("${ENGINE_DIRECTORY}/Graphics/assets/shaders/postProcess.kasset") })
	{
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, shader->GetPipeline());
		CmdPushConstants(commandBuffer, nullptr, frameIndex);
		vkCmdDraw(commandBuffer, 3, 1, 0, 0);
	}
}

void GRenderer::CmdBeginRenderingInterface(VkCommandBuffer commandBuffer, const u32 frameIndex) const
{
	const VkRenderingAttachmentInfo swapchainAttachment{
		.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
		.imageView = SwapChain.GetData(frameDatas_[frameIndex].imageIndex).imageView,
		.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,

		.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD,
		.storeOp = VK_ATTACHMENT_STORE_OP_STORE,
	};

	const VkRenderingInfo swapchainRenderingInfo{
		.sType = VK_STRUCTURE_TYPE_RENDERING_INFO,
		.renderArea{
			.offset = { 0, 0 },
			.extent = SwapChain.GetExtent()
		},
		.layerCount = 1,
		.colorAttachmentCount = 1,
		.pColorAttachments = &swapchainAttachment,
	};

	vkCmdBeginRendering(commandBuffer, &swapchainRenderingInfo);
}

void GRenderer::CmdDrawFrameInterface(VkCommandBuffer commandBuffer, const u32 frameIndex) const
{
	std::map<i32, std::vector<const UDrawCall*>> layerDrawCalls{};

	for (const auto* drawCall : interfaceDrawCalls_)
	{
		layerDrawCalls[static_cast<i32>(drawCall->sortKey)].push_back(drawCall);
	}

	VkPipeline currentPipeline{ VK_NULL_HANDLE };

	IndexBuffer.CmdBind(commandBuffer);

	for (const auto& drawCalls : layerDrawCalls | std::views::values)
	{
		for (const auto* drawCall : drawCalls)
		{
			if (currentPipeline != drawCall->pipeline)
			{
				currentPipeline = drawCall->pipeline;
				vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, drawCall->pipeline);
			}

			CmdPushConstants(commandBuffer, drawCall, frameIndex);
			vkCmdSetScissor(commandBuffer, 0, 1, &drawCall->scissor);
			CmdDraw(commandBuffer, drawCall);
		}
	}
}

void GRenderer::CmdBarrierSwapchainWriteToPresent(VkCommandBuffer commandBuffer, const u32 frameIndex) const
{
	CmdTransitionImages(commandBuffer
		, &SwapChain.GetData(frameDatas_[frameIndex].imageIndex).image, 1
		, VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT
		, VK_PIPELINE_STAGE_2_NONE
		, VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT
		, VK_ACCESS_2_NONE
		, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
		, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
		, { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 }
	);
}

void GRenderer::CmdPushConstants(VkCommandBuffer commandBuffer, const UDrawCall* drawCall, const u32 frameIndex) const
{
	const UPushConstants pc{
		.frameContextBufferAddress = FrameContextBuffer.GetAddress(frameIndex),
		.drawIndex = drawCall ? drawCall->index : 0,
		.directionalIndex = 0,
	};

	vkCmdPushConstants(commandBuffer
		, PipelineResourceManager.GetPipelineLayout()
		, VK_SHADER_STAGE_ALL
		, 0
		, sizeof(UPushConstants)
		, &pc
	);
}

void GRenderer::CmdDraw(VkCommandBuffer commandBuffer, const UDrawCall* drawCall) const
{
	if (!drawCall)
	{
		return;
	}

	vkCmdDrawIndexed(commandBuffer
		, drawCall->indexCount
		, 1
		, drawCall->firstIndex
		, 0
		, 0
	);
}

void GRenderer::CmdEndRendering(VkCommandBuffer commandBuffer) const
{
	vkCmdEndRendering(commandBuffer);
}

void GRenderer::EndCommandBuffer(VkCommandBuffer commandBuffer) const
{
	VK_CHECK_THROW(
		vkEndCommandBuffer(commandBuffer),
		"failed to record command buffer!"
	);
}

void GRenderer::SubmitCommandBuffer(const u32 frameIndex)
{
	const VkCommandBufferSubmitInfo cmdBufInfo{
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO,
		.commandBuffer = frameDatas_[frameIndex].commandBuffer,
		.deviceMask = 0
	};

	const VkSemaphoreSubmitInfo waitSemaphoreInfo{
		.sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO,
		.semaphore = frameDatas_[frameIndex].imageAvailableSemaphore,
		.value = 0,
		.stageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
		.deviceIndex = 0,
	};

	const VkSemaphoreSubmitInfo signalSemaphoreInfo{
		.sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO,
		.semaphore = frameDatas_[frameIndex].renderFinishedSemaphore,
		.value = 0,
		.stageMask = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT,
		.deviceIndex = 0,
	};

	const VkSubmitInfo2 submitInfo2{
		.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO_2,

		.waitSemaphoreInfoCount = 1,
		.pWaitSemaphoreInfos = &waitSemaphoreInfo,

		.commandBufferInfoCount = 1,
		.pCommandBufferInfos = &cmdBufInfo,

		.signalSemaphoreInfoCount = 1,
		.pSignalSemaphoreInfos = &signalSemaphoreInfo
	};
	VK_CHECK_THROW(
		vkQueueSubmit2(Context.GetGraphicsQueue(), 1, &submitInfo2, frameDatas_[frameIndex].inFlightFence),
		"failed to submit draw command buffer!"
	);

	const VkPresentInfoKHR presentInfo{
		.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,

		.waitSemaphoreCount = 1,
		.pWaitSemaphores = &frameDatas_[frameIndex].renderFinishedSemaphore,

		.swapchainCount = 1,
		.pSwapchains = &SwapChain.GetSwapChain(),
		.pImageIndices = &frameDatas_[frameIndex].imageIndex,
	};
	const VkResult result{ vkQueuePresentKHR(Context.GetPresentQueue(), &presentInfo) };

	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
	{
		RecreateFrame();
	}
	else VK_CHECK_THROW(
		result,
		"failed to present swap chain image!"
	);

	if constexpr (IS_MULTI_THREADED)
	{
		if (!TryAcquireNextImage(frameIndex))
		{
			KT_LOG(ELogImportanceLevel::High, "Graphics", "frame {0} skipped", frameCount_);
			return;
		}
	}
}

void GRenderer::JoinThread(std::thread& thread) const
{
	if (thread.joinable())
	{
		thread.join();
	}
}

u32 GRenderer::GetGameThreadFrame() const
{
	// Prepare game thread for render thread
	return frameCount_ % static_cast<u32>(KT_FRAMES_IN_FLIGHT);
}

u32 GRenderer::GetRenderThreadFrame() const
{
	// Prepare render thread for RHI thread
	return ((frameCount_ + KT_FRAMES_IN_FLIGHT) - 1) % static_cast<u32>(KT_FRAMES_IN_FLIGHT); // avoid negative with + KT_FRAMES_IN_FLIGHT
}

u32 GRenderer::GetRHIThreadFrame() const
{
	// Prepare RHI thread for game thread
	return ((frameCount_ + KT_FRAMES_IN_FLIGHT) - 2) % static_cast<u32>(KT_FRAMES_IN_FLIGHT); // avoid negative with + KT_FRAMES_IN_FLIGHT
}

void GRenderer::RecreateFrame()
{
	// Wait for CPU
	JoinThread(renderThread_);
	JoinThread(rhiThread_);

	// Wait for GPU
	vkDeviceWaitIdle(Context.GetDevice());

	FrameContextBuffer.UnregisterGBufferTextures();

	CleanupImageResources();
	
	SwapChain.Cleanup();
	SwapChain.Init();

	CreateImageResources();

	FrameContextBuffer.RegisterGBufferTextures();

	for (const auto& frameData : frameDatas_)
	{
		vkResetCommandPool(Context.GetDevice(), frameData.commandPool, 0);
	}
}
