#include "SceneRender.h"

#include "Barriers.h"
#include "DrawCommand.h"
#include "DrawDataBufferData.h"
#include "IndexBuffer.h"
#include "MaterialBufferData.h"
#include "ParametersBufferData.h"
#include "PipelineResourceManager.h"
#include "PushConstants.h"
#include "TransformBufferData.h"
#include <array>
#include <kotono_platform/Device.h>
#include <kotono_platform/Swapchain.h>
#include <ranges>

static constexpr u32 MAX_DRAW_DATAS{ 65536 };

USceneRender::USceneRender(UDevice& device, USwapchain& swapchain, UPipelineResourceManager& pipelineResourceManager)
	: device_{ device }
	, swapchain_{ swapchain }
	, pipelineResourceManager_{ pipelineResourceManager }
	, frameContextBuffer_{ device }
	, gpuBuffers_{ device }
	, lightBuffers_{ device, pipelineResourceManager }
{
}

void USceneRender::Init(glm::uvec2 const& extent)
{
	extent_ = { extent.x, extent.y };

	frameContextBuffer_.Init();

	CreateImageResources(device_.GetDepthFormat());
	RegisterFrameContextBufferTextures();

	drawDataBuffer_ = device_.CreateAllocatedBuffer(
		  sizeof(UDrawDataBufferData) * MAX_DRAW_DATAS
		, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT
		| VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT
		, VMA_ALLOCATION_CREATE_MAPPED_BIT
		| VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT
	);
	transformBuffer_ = device_.CreateAllocatedBuffer(
		  sizeof(UTransformBufferData) * MAX_DRAW_DATAS
		, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT
		| VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT
		, VMA_ALLOCATION_CREATE_MAPPED_BIT
		| VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT
	);
	parametersBuffer_ = device_.CreateAllocatedBuffer(
		  sizeof(UParametersBufferData) * MAX_DRAW_DATAS
		, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT
		| VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT
		, VMA_ALLOCATION_CREATE_MAPPED_BIT
		| VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT
	);
	materialBuffer_ = device_.CreateAllocatedBuffer(
		  sizeof(UMaterialBufferData) * MAX_DRAW_DATAS
		, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT
		| VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT
		, VMA_ALLOCATION_CREATE_MAPPED_BIT
		| VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT
	);

	lightBuffers_.Init();
	gpuBuffers_.Init();

	isAABBDirty_ = true;
}

void USceneRender::Cleanup() const
{
	frameContextBuffer_.Cleanup();

	CleanupImageResources();
	UnregisterFrameContextBufferTextures();

	device_.CleanupAllocatedBuffer(drawDataBuffer_);
	device_.CleanupAllocatedBuffer(transformBuffer_);
	device_.CleanupAllocatedBuffer(parametersBuffer_);
	device_.CleanupAllocatedBuffer(materialBuffer_);

	lightBuffers_.Cleanup();
	gpuBuffers_.Cleanup();
}

u32 USceneRender::GetRenderTarget() const
{
	return postProcessIndex_;
}

u32 USceneRender::GetDirectionalLightShadowMapTargetIndex(u32 index) const
{
	return lightBuffers_.GetDirectionalLightShadowMapTargetIndex(index);
}

void USceneRender::UpdateBuffers(
	  UFrameContextSceneView const& sceneView
	, std::span<UDrawCommand const> drawCommands
	, std::span<UDirectionalLight const> directionalLights
	, std::span<UPointLight const> pointLights
	, u32 samplerIndex
) const
{
	frameContextBuffer_.UpdateBuffer(
		  sceneView
		, MakeFrameContextAddresses()
		, MakeFrameContextTargets()
		, directionalLights.size()
		, pointLights.size()
		, samplerIndex
	);

	drawDataBuffer_.UpdateMappedData<UDrawDataBufferData>(MakeDrawDataBuffer(drawCommands));
	transformBuffer_.UpdateMappedData<UTransformBufferData>(MakeTransformBuffer(drawCommands));
	parametersBuffer_.UpdateMappedData<UParametersBufferData>(MakeParametersBuffer(drawCommands));
	materialBuffer_.UpdateMappedData<UMaterialBufferData>(MakeMaterialBuffer(drawCommands));

	lightBuffers_.UpdateBuffers(directionalLights, pointLights);
}

void USceneRender::CmdDraw(USceneRenderContext const& renderContext, USceneRenderData const& renderData) const
{
	if (isAABBDirty_)
	{
		isAABBDirty_ = false;
		CmdUpdateClusterAABB(renderContext);
	}

	// Light binning 
	// - Wait for fill buffer command to be executable
	CmdBarrierComputeFragmentReadToClearWrite(renderContext.commandBuffer);
	// - Atomic reset light counter to 0
	CmdResetLightCounter(renderContext.commandBuffer);
	// - Make light counter accessible and light binning writable
	CmdBarrierComputeClearWriteToReadWrite(renderContext.commandBuffer);
	// - Compute light binning
	CmdDispatchLightBinning(renderContext);
	// - Make light binning accessible to fragment
	CmdBarrierComputeWriteToFragmentRead(renderContext.commandBuffer);

	// Shadow-maps
	// - Set rendering area to fit shadow-maps
	lightBuffers_.CmdSetViewportAndScissor(renderContext.commandBuffer);
	// - Make shadow maps writable
	lightBuffers_.CmdBarrierShadowMapsNoneToWrite(renderContext.commandBuffer);
	// - Generate shadow-maps
	CmdDrawFrameShadowMaps(renderContext, renderData);

	// Reset the rendering area to full screen
	CmdUseViewport(renderContext.commandBuffer);

	// Depth pre-pass
	// - Make depth writable
	CmdBarrierDepthNoneToWrite(renderContext.commandBuffer);
	// - Write depth
	CmdBeginRenderingDepthPrePass(renderContext.commandBuffer);
	CmdDrawFrameDepthPrePass(renderContext, renderData);
	CmdEndRendering(renderContext.commandBuffer);

	// G-Buffer
	// - Make depth readable
	CmdBarrierDepthWriteToRead(renderContext.commandBuffer);
	// - Make G-Buffer writable
	CmdBarrierGBufferNoneToWrite(renderContext.commandBuffer);
	// - Write G-Buffer
	CmdBeginRenderingGBuffer(renderContext.commandBuffer);
	CmdDrawFrameGBuffer(renderContext, renderData);
	CmdEndRendering(renderContext.commandBuffer);
	// - Make G-Buffer readable
	CmdBarrierGBufferWriteToRead(renderContext.commandBuffer);

	// Deferred lighting
	// - Make shadow-maps shader-readable
	lightBuffers_.CmdBarrierShadowMapsWriteToShaderRead(renderContext.commandBuffer);
	// - Make depth shader-readable (reconstruct world pos)
	CmdBarrierDepthReadToShaderRead(renderContext.commandBuffer);
	// - Make color target writable
	CmdBarrierColorNoneToWrite(renderContext.commandBuffer);
	// - Write color target
	CmdBeginRenderingDeferredLighting(renderContext.commandBuffer);
	CmdDrawFrameDeferredLighting(renderContext);
	CmdEndRendering(renderContext.commandBuffer);
	// - Make color target readable
	CmdBarrierColorWriteToRead(renderContext.commandBuffer);

	// Post-process
	// - Make post-process image writable
	CmdBarrierPostProcessNoneToWrite(renderContext.commandBuffer);
	// - Write post-process image
	CmdBeginRenderingPostProcess(renderContext.commandBuffer);
	CmdDrawFramePostProcess(renderContext);
	CmdEndRendering(renderContext.commandBuffer);
	// - Make post-process image readable
	CmdBarrierPostProcessWriteToRead(renderContext.commandBuffer);
}

void USceneRender::CreateImageResources(VkFormat depthFormat)
{
	auto const [width, height] { extent_ };
	colorTarget_ =			device_.CreateAllocatedImage(UAllocatedImageCreateInfo::CreateSampled2D(width, height, 1, VK_FORMAT_R16G16B16A16_SFLOAT,	VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,			VK_IMAGE_ASPECT_COLOR_BIT));
	albedoTarget_ =			device_.CreateAllocatedImage(UAllocatedImageCreateInfo::CreateSampled2D(width, height, 1, VK_FORMAT_R8G8B8A8_SRGB,			VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,			VK_IMAGE_ASPECT_COLOR_BIT));
	normalTarget_ =			device_.CreateAllocatedImage(UAllocatedImageCreateInfo::CreateSampled2D(width, height, 1, VK_FORMAT_R16G16B16A16_SFLOAT,	VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,			VK_IMAGE_ASPECT_COLOR_BIT));
	ormTarget_ =			device_.CreateAllocatedImage(UAllocatedImageCreateInfo::CreateSampled2D(width, height, 1, VK_FORMAT_R8G8B8A8_UNORM,			VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,			VK_IMAGE_ASPECT_COLOR_BIT));
	depthTarget_ =			device_.CreateAllocatedImage(UAllocatedImageCreateInfo::CreateSampled2D(width, height, 1, depthFormat,						VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,	VK_IMAGE_ASPECT_DEPTH_BIT));
	postProcessTarget_ =	device_.CreateAllocatedImage(UAllocatedImageCreateInfo::CreateSampled2D(width, height, 1, swapchain_.GetFormat(),			VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,			VK_IMAGE_ASPECT_COLOR_BIT));
}

void USceneRender::CleanupImageResources() const
{
	device_.CleanupAllocatedImage(depthTarget_);
	device_.CleanupAllocatedImage(albedoTarget_);
	device_.CleanupAllocatedImage(normalTarget_);
	device_.CleanupAllocatedImage(ormTarget_);
	device_.CleanupAllocatedImage(colorTarget_);
	device_.CleanupAllocatedImage(postProcessTarget_);
}

void USceneRender::RegisterFrameContextBufferTextures()
{
	depthIndex_ = pipelineResourceManager_.RegisterTexture(depthTarget_.imageView);
	albedoIndex_ = pipelineResourceManager_.RegisterTexture(albedoTarget_.imageView);
	normalIndex_ = pipelineResourceManager_.RegisterTexture(normalTarget_.imageView);
	ormIndex_ = pipelineResourceManager_.RegisterTexture(ormTarget_.imageView);
	colorIndex_ = pipelineResourceManager_.RegisterTexture(colorTarget_.imageView);
	postProcessIndex_ = pipelineResourceManager_.RegisterTexture(postProcessTarget_.imageView);
}

void USceneRender::UnregisterFrameContextBufferTextures() const
{
	pipelineResourceManager_.UnregisterTexture(depthIndex_);
	pipelineResourceManager_.UnregisterTexture(albedoIndex_);
	pipelineResourceManager_.UnregisterTexture(normalIndex_);
	pipelineResourceManager_.UnregisterTexture(ormIndex_);
	pipelineResourceManager_.UnregisterTexture(colorIndex_);
	pipelineResourceManager_.UnregisterTexture(postProcessIndex_);
}

UFrameContextAddresses USceneRender::MakeFrameContextAddresses() const
{
	return {
		.drawDataBufferAddress = drawDataBuffer_.bda,
		.materialBufferAddress = materialBuffer_.bda,
		.transformBufferAddress = transformBuffer_.bda,
		.parametersBufferAddress = parametersBuffer_.bda,

		.directionalLightBufferAddress = lightBuffers_.GetDirectionalLightAddress(),
		.pointLightBufferAddress = lightBuffers_.GetPointLightAddress(),

		.clusterAABBBufferAddress = gpuBuffers_.GetClusterAABBAddress(),
		.clusterGridBufferAddress = gpuBuffers_.GetClusterGridAddress(),
		.lightIndexBufferAddress = gpuBuffers_.GetLightIndexAddress(),
		.lightCounterBufferAddress = gpuBuffers_.GetLightCounterAddress(),
	};
}

UFrameContextTargets USceneRender::MakeFrameContextTargets() const
{
	return UFrameContextTargets{
		.gBufferDepth = depthIndex_,
		.gBufferAlbedo = albedoIndex_,
		.gBufferNormal = normalIndex_,
		.gBufferORM = ormIndex_,
		.colorTarget = colorIndex_,
	};
}

std::vector<UDrawDataBufferData> USceneRender::MakeDrawDataBuffer(std::span<UDrawCommand const> drawCommands) const
{
	return drawCommands
		| std::views::transform([](UDrawCommand const& drawCommand) {
			return UDrawDataBufferData{
				.materialIndex = drawCommand.drawIndex,
				.transformIndex = drawCommand.drawIndex,
				.parametersIndex = drawCommand.drawIndex,
				.vertexBufferAddress = drawCommand.vertexBufferAddress,
			};
		})
		| std::ranges::to<std::vector>();
}

std::vector<UTransformBufferData> USceneRender::MakeTransformBuffer(std::span<UDrawCommand const> drawCommands) const
{
	return drawCommands
		| std::views::transform([](UDrawCommand const& drawCommand) {
			return UTransformBufferData{
				.modelMatrix = drawCommand.modelMatrix,
				.normalMatrix = drawCommand.normalMatrix,
			};
		})
		| std::ranges::to<std::vector>();
}

std::vector<UParametersBufferData> USceneRender::MakeParametersBuffer(std::span<UDrawCommand const> drawCommands) const
{
	return drawCommands
		| std::views::transform([](UDrawCommand const& drawCommand) {
			return UParametersBufferData{
				.scalars = drawCommand.scalars,
				.vectors = drawCommand.vectors,
				.textures = drawCommand.textures,
			};
		})
		| std::ranges::to<std::vector>();
}

std::vector<UMaterialBufferData> USceneRender::MakeMaterialBuffer(std::span<UDrawCommand const> drawCommands) const
{
	return drawCommands
		| std::views::transform(&UDrawCommand::material)
		| std::views::transform([](UDrawCommand::Material const& material) {
			return UMaterialBufferData{
				.albedoIndex = material.albedoIndex,
				.normalIndex = material.normalIndex,
				.ormIndex = material.ormIndex,
				.emissiveIndex = material.emissiveIndex,
				.materialType = material.materialType,
				.samplerIndex = material.samplerIndex,
			};
		})
		| std::ranges::to<std::vector>();
}

void USceneRender::CmdUpdateClusterAABB(USceneRenderContext const& renderContext) const
{
	vkCmdBindPipeline(renderContext.commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, renderContext.clusterAABBPipeline);
	CmdPushConstants(renderContext.commandBuffer, 0, 0);
	vkCmdDispatch(renderContext.commandBuffer, 16, 9, 24); // todo: variable
}

void USceneRender::CmdBarrierComputeFragmentReadToClearWrite(VkCommandBuffer commandBuffer) const
{
	Barriers::CmdTransitionCompute(commandBuffer
		, VK_PIPELINE_STAGE_2_FRAGMENT_SHADER_BIT
		, VK_PIPELINE_STAGE_2_CLEAR_BIT
		, VK_ACCESS_2_SHADER_READ_BIT
		, VK_ACCESS_2_TRANSFER_WRITE_BIT
	);
}

void USceneRender::CmdResetLightCounter(VkCommandBuffer commandBuffer) const
{
	vkCmdFillBuffer(commandBuffer
		, gpuBuffers_.GetLightCounterBuffer()
		, 0
		, sizeof(u32)
		, 0
	);
}

void USceneRender::CmdBarrierComputeClearWriteToReadWrite(VkCommandBuffer commandBuffer) const
{
	Barriers::CmdTransitionCompute(commandBuffer
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

void USceneRender::CmdDispatchLightBinning(USceneRenderContext const& renderContext) const
{
	vkCmdBindPipeline(renderContext.commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, renderContext.lightBinningPipeline);
	CmdPushConstants(renderContext.commandBuffer, 0, 0);
	vkCmdDispatch(renderContext.commandBuffer, 16, 9, 24);
}

void USceneRender::CmdBarrierComputeWriteToFragmentRead(VkCommandBuffer commandBuffer) const
{
	Barriers::CmdTransitionCompute(commandBuffer
		, VK_PIPELINE_STAGE_2_COMPUTE_SHADER_BIT
		, VK_PIPELINE_STAGE_2_FRAGMENT_SHADER_BIT
		, VK_ACCESS_2_SHADER_WRITE_BIT
		, VK_ACCESS_2_SHADER_READ_BIT
	);
}

void USceneRender::CmdDrawFrameShadowMaps(USceneRenderContext const& renderContext, USceneRenderData const& renderData) const
{
	renderContext.indexBuffer.CmdBind(renderContext.commandBuffer);

	vkCmdBindPipeline(renderContext.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, renderContext.shadowPrePassPipeline);

	for (u32 i{ 0 }; i < renderData.directionalLightCount; ++i)
	{
		lightBuffers_.CmdBeginRenderingShadowMapTarget(renderContext.commandBuffer, i);
		// todo: add cast shadow check
		CmdDrawFrame(renderContext.commandBuffer, renderData.drawCommands, i);

		CmdEndRendering(renderContext.commandBuffer);
	}
}

void USceneRender::CmdBarrierDepthNoneToWrite(VkCommandBuffer commandBuffer) const
{
	Barriers::CmdTransitionImages(commandBuffer
		, std::array{ depthTarget_.image }
		, VK_PIPELINE_STAGE_2_NONE
		, VK_PIPELINE_STAGE_2_EARLY_FRAGMENT_TESTS_BIT
		| VK_PIPELINE_STAGE_2_LATE_FRAGMENT_TESTS_BIT
		, VK_ACCESS_2_NONE
		, VK_ACCESS_2_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT
		, VK_IMAGE_LAYOUT_UNDEFINED
		, VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL
		, { VK_IMAGE_ASPECT_DEPTH_BIT, 0, 1, 0, 1 }
	);
}

void USceneRender::CmdBeginRenderingDepthPrePass(VkCommandBuffer commandBuffer) const
{
	const VkRenderingAttachmentInfo depthAttachment{
		.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,

		.imageView = depthTarget_.imageView,
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
			.extent = extent_
		},
		.layerCount = 1,
		.colorAttachmentCount = 0,
		.pDepthAttachment = &depthAttachment,
	};
	vkCmdBeginRendering(commandBuffer, &renderInfo);
}

void USceneRender::CmdDrawFrameDepthPrePass(USceneRenderContext const& renderContext, USceneRenderData const& renderData) const
{
	renderContext.indexBuffer.CmdBind(renderContext.commandBuffer);
	vkCmdBindPipeline(renderContext.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, renderContext.depthPrePassPipeline);
	CmdDrawFrame(renderContext.commandBuffer, renderData.drawCommands, 0);
}

void USceneRender::CmdBarrierDepthWriteToRead(VkCommandBuffer commandBuffer) const
{
	Barriers::CmdTransitionImages(commandBuffer
		, std::array{ depthTarget_.image }
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

void USceneRender::CmdBarrierGBufferNoneToWrite(VkCommandBuffer commandBuffer) const
{
	std::array const images{
		albedoTarget_.image,
		normalTarget_.image,
		ormTarget_.image,
	};
	Barriers::CmdTransitionImages(commandBuffer
		, images
		, VK_PIPELINE_STAGE_2_NONE
		, VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT
		, VK_ACCESS_2_NONE
		, VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT
		, VK_IMAGE_LAYOUT_UNDEFINED
		, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
		, { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 }
	);
}

void USceneRender::CmdBeginRenderingGBuffer(VkCommandBuffer commandBuffer) const
{
	constexpr auto makeRenderingAttachmentInfo{ [](VkImageView imageView) {
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
		makeRenderingAttachmentInfo(albedoTarget_.imageView),
		makeRenderingAttachmentInfo(normalTarget_.imageView),
		makeRenderingAttachmentInfo(ormTarget_.imageView),
	};

	const VkRenderingAttachmentInfo depthAttachment{
		.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,

		.imageView = depthTarget_.imageView,
		.imageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL,

		.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD,
		.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE, // sure ?
		.clearValue{
			.depthStencil = { 0.0f, 0 }, // 0.0f instead of 1.0f because of reverse depth
		},
	};

	const VkRenderingInfo renderingInfo{
		.sType = VK_STRUCTURE_TYPE_RENDERING_INFO,
		.renderArea{
			.offset = { 0, 0 },
			.extent = extent_,
		},
		.layerCount = 1,
		.colorAttachmentCount = static_cast<u32>(colorAttachments.size()),
		.pColorAttachments = colorAttachments.data(),
		.pDepthAttachment = &depthAttachment,
	};

	vkCmdBeginRendering(commandBuffer, &renderingInfo);
}

void USceneRender::CmdDrawFrameGBuffer(USceneRenderContext const& renderContext, USceneRenderData const& renderData) const
{
	std::unordered_map<VkPipeline, std::vector<UDrawCommand>> pipelineDrawCommands{};

	for (auto const& drawCommand : renderData.drawCommands)
	{
		pipelineDrawCommands[drawCommand.pipeline].push_back(drawCommand);
	}

	renderContext.indexBuffer.CmdBind(renderContext.commandBuffer);

	for (auto const& [pipeline, drawCommands] : pipelineDrawCommands)
	{
		vkCmdBindPipeline(renderContext.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
		CmdDrawFrame(renderContext.commandBuffer, drawCommands, 0);
	}
}

void USceneRender::CmdBarrierGBufferWriteToRead(VkCommandBuffer commandBuffer) const
{
	std::array const images{
		albedoTarget_.image,
		normalTarget_.image,
		ormTarget_.image,
	};
	Barriers::CmdTransitionImages(commandBuffer
		, images
		, VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT
		, VK_PIPELINE_STAGE_2_FRAGMENT_SHADER_BIT
		, VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT
		, VK_ACCESS_2_SHADER_SAMPLED_READ_BIT
		, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
		, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
		, { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 }
	);
}

void USceneRender::CmdBarrierDepthReadToShaderRead(VkCommandBuffer commandBuffer) const
{
	Barriers::CmdTransitionImages(commandBuffer
		, std::array{ depthTarget_.image }
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

void USceneRender::CmdBarrierColorNoneToWrite(VkCommandBuffer commandBuffer) const
{
	Barriers::CmdTransitionImages(commandBuffer
		, std::array{ colorTarget_.image }
		, VK_PIPELINE_STAGE_2_NONE
		, VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT
		, VK_ACCESS_2_NONE
		, VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT
		, VK_IMAGE_LAYOUT_UNDEFINED
		, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
		, { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 }
	);
}

void USceneRender::CmdBeginRenderingDeferredLighting(VkCommandBuffer commandBuffer) const
{
	const VkRenderingAttachmentInfo colorAttachment{
		.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
		.imageView = colorTarget_.imageView,
		.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,

		.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
		.storeOp = VK_ATTACHMENT_STORE_OP_STORE,
	};
	const VkRenderingInfo renderInfo{
		.sType = VK_STRUCTURE_TYPE_RENDERING_INFO,
		.renderArea{
			.offset = { 0, 0 },
			.extent = extent_,
		},
		.layerCount = 1,
		.colorAttachmentCount = 1,
		.pColorAttachments = &colorAttachment,
		.pDepthAttachment = VK_NULL_HANDLE,
	};
	vkCmdBeginRendering(commandBuffer, &renderInfo);
}

void USceneRender::CmdDrawFrameDeferredLighting(USceneRenderContext const& renderContext) const
{
	vkCmdBindPipeline(renderContext.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, renderContext.deferredLightingPipeline);
	CmdPushConstants(renderContext.commandBuffer, 0, 0);
	vkCmdDraw(renderContext.commandBuffer, 3, 1, 0, 0);
}

void USceneRender::CmdBarrierColorWriteToRead(VkCommandBuffer commandBuffer) const
{
	Barriers::CmdTransitionImages(commandBuffer
		, std::array{ colorTarget_.image }
		, VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT
		, VK_PIPELINE_STAGE_2_FRAGMENT_SHADER_BIT
		, VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT
		, VK_ACCESS_2_SHADER_SAMPLED_READ_BIT
		, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
		, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
		, { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 }
	);
}

void USceneRender::CmdBarrierPostProcessNoneToWrite(VkCommandBuffer commandBuffer) const
{
	Barriers::CmdTransitionImages(commandBuffer
		, std::array{ postProcessTarget_.image }
		, VK_PIPELINE_STAGE_2_NONE
		, VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT
		, VK_ACCESS_2_NONE
		, VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT
		, VK_IMAGE_LAYOUT_UNDEFINED
		, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
		, { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 }
	);
}

void USceneRender::CmdBeginRenderingPostProcess(VkCommandBuffer commandBuffer) const
{
	VkRenderingAttachmentInfo const swapchainAttachment{
		.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
		.imageView = postProcessTarget_.imageView,
		.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,

		.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
		.storeOp = VK_ATTACHMENT_STORE_OP_STORE,
	};

	VkRenderingInfo const swapchainRenderingInfo{
		.sType = VK_STRUCTURE_TYPE_RENDERING_INFO,
		.renderArea{
			.offset = { 0, 0 },
			.extent = extent_
		},
		.layerCount = 1,
		.colorAttachmentCount = 1,
		.pColorAttachments = &swapchainAttachment,
	};

	vkCmdBeginRendering(commandBuffer, &swapchainRenderingInfo);
}

void USceneRender::CmdDrawFramePostProcess(USceneRenderContext const& renderContext) const
{
	vkCmdBindPipeline(renderContext.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, renderContext.postProcessPipeline);
	CmdPushConstants(renderContext.commandBuffer, 0, 0);
	vkCmdDraw(renderContext.commandBuffer, 3, 1, 0, 0);
}

void USceneRender::CmdBarrierPostProcessWriteToRead(VkCommandBuffer commandBuffer) const
{
	Barriers::CmdTransitionImages(commandBuffer
		, std::array{ postProcessTarget_.image }
		, VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT
		, VK_PIPELINE_STAGE_2_FRAGMENT_SHADER_BIT
		, VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT
		, VK_ACCESS_2_SHADER_SAMPLED_READ_BIT
		, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
		, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
		, { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 }
	);
}

void USceneRender::CmdEndRendering(VkCommandBuffer commandBuffer) const
{
	vkCmdEndRendering(commandBuffer);
}

void USceneRender::CmdUseViewport(VkCommandBuffer commandBuffer) const
{
	VkViewport const viewport{
		.x = 0.0f,
		.y = 0.0f,
		.width = static_cast<f32>(extent_.width),
		.height = static_cast<f32>(extent_.height),
		.minDepth = 0.0f,
		.maxDepth = 1.0f,
	};
	VkRect2D const scissor{
		.offset = { 0, 0 },
		.extent = extent_,
	};

	vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
	vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
}

void USceneRender::CmdDrawFrame(VkCommandBuffer commandBuffer, std::span<UDrawCommand const> drawCommands, u32 directionalIndex) const
{
	for (auto const& drawCommand : drawCommands)
	{
		CmdPushConstants(commandBuffer, drawCommand.drawIndex, directionalIndex);
		
		vkCmdDrawIndexed(commandBuffer
			, drawCommand.indexCount
			, 1
			, drawCommand.firstIndex
			, 0
			, 0
		);
	}
}

void USceneRender::CmdPushConstants(VkCommandBuffer commandBuffer, u32 drawIndex, u32 directionalIndex) const
{
	const UPushConstants pc{
		.frameContextBufferAddress = frameContextBuffer_.GetAddress(),
		.drawIndex = drawIndex,
		.directionalIndex = directionalIndex,
	};

	vkCmdPushConstants(commandBuffer
		, pipelineResourceManager_.GetPipelineLayout()
		, VK_SHADER_STAGE_ALL
		, 0
		, sizeof(UPushConstants)
		, &pc
	);
}
