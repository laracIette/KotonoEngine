#include "SceneRender.h"

#include "Barriers.h"
#include "DrawCommand.h"
#include "IndexBuffer.h"
#include "MaterialBuffer.h"
#include "PipelineResourceManager.h"
#include "PushConstants.h"
#include "Sampler.h"
#include "Shader.h"
#include <array>
#include <kotono_common/AssetManager.h>
#include <kotono_common/log.h>
#include <kotono_platform/Context.h>
#include <ranges>

void USceneRender::Init(glm::uvec2 const& extent, VkFormat swapChainFormat)
{
	extent_ = { extent.x, extent.y };

	frameContextBuffer_.Init();

	CreateImageResources(swapChainFormat);
	RegisterFrameContextBufferTextures();

	drawDataBuffer_.Init();
	transformBuffer_.Init();
	parametersBuffer_.Init();

	lightBuffers_.Init();
	gpuBuffers_.Init();

	isAABBDirty_ = true;

	CreateSampler();
}

void USceneRender::Cleanup() const
{
	frameContextBuffer_.Cleanup();

	CleanupImageResources();
	UnregisterFrameContextBufferTextures();

	drawDataBuffer_.Cleanup();
	transformBuffer_.Cleanup();
	parametersBuffer_.Cleanup();

	lightBuffers_.Cleanup();
	gpuBuffers_.Cleanup();
}

void USceneRender::SetExtent(glm::uvec2 const& extent, VkFormat swapChainFormat)
{
	extent_ = { extent.x, extent.y };
	isAABBDirty_ = true;

	UnregisterFrameContextBufferTextures();
	CleanupImageResources();

	CreateImageResources(swapChainFormat);
	RegisterFrameContextBufferTextures();
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
) const
{
	frameContextBuffer_.UpdateBuffer(
		  sceneView
		, MakeFrameContextAddresses()
		, MakeFrameContextTargets()
		, directionalLights.size()
		, pointLights.size()
		, samplerIndex_
	);

	drawDataBuffer_.UpdateBuffer(MakeDrawDataBuffer(drawCommands));
	transformBuffer_.UpdateBuffer(MakeTransformBuffer(drawCommands));
	parametersBuffer_.UpdateBuffer(MakeParametersBuffer(drawCommands));

	lightBuffers_.UpdateBuffers(sceneView, directionalLights, pointLights);
}

void USceneRender::CmdDraw(VkCommandBuffer commandBuffer, std::span<UDrawCommand const> drawCommands, u32 directionalLightCount) const
{
	if (isAABBDirty_)
	{
		isAABBDirty_ = false;
		CmdUpdateClusterAABB(commandBuffer);
	}

	// Light binning 
	// - Wait for fill buffer command to be executable
	CmdBarrierComputeFragmentReadToClearWrite(commandBuffer);
	// - Atomic reset light counter to 0
	CmdResetLightCounter(commandBuffer);
	// - Make light counter accessible and light binning writable
	CmdBarrierComputeClearWriteToReadWrite(commandBuffer);
	// - Compute light binning
	CmdDispatchLightBinning(commandBuffer);
	// - Make light binning accessible to fragment
	CmdBarrierComputeWriteToFragmentRead(commandBuffer);

	// Shadow-maps
	// - Set rendering area to fit shadow-maps
	lightBuffers_.CmdSetViewportAndScissor(commandBuffer);
	// - Make shadow maps writable
	lightBuffers_.CmdBarrierShadowMapsNoneToWrite(commandBuffer);
	// - Generate shadow-maps
	CmdDrawFrameShadowMaps(commandBuffer, drawCommands, directionalLightCount);

	// Reset the rendering area to full screen
	CmdUseViewport(commandBuffer);

	// Depth pre-pass
	// - Make depth writable
	CmdBarrierDepthNoneToWrite(commandBuffer);
	// - Write depth
	CmdBeginRenderingDepthPrePass(commandBuffer);
	CmdDrawFrameDepthPrePass(commandBuffer, drawCommands);
	CmdEndRendering(commandBuffer);

	// G-Buffer
	// - Make depth readable
	CmdBarrierDepthWriteToRead(commandBuffer);
	// - Make G-Buffer writable
	CmdBarrierGBufferNoneToWrite(commandBuffer);
	// - Write G-Buffer
	CmdBeginRenderingGBuffer(commandBuffer);
	CmdDrawFrameGBuffer(commandBuffer, drawCommands);
	CmdEndRendering(commandBuffer);
	// - Make G-Buffer readable
	CmdBarrierGBufferWriteToRead(commandBuffer);

	// Deferred lighting
	// - Make shadow-maps shader-readable
	lightBuffers_.CmdBarrierShadowMapsWriteToShaderRead(commandBuffer);
	// - Make depth shader-readable (reconstruct world pos)
	CmdBarrierDepthReadToShaderRead(commandBuffer);
	// - Make color target writable
	CmdBarrierColorNoneToWrite(commandBuffer);
	// - Write color target
	CmdBeginRenderingDeferredLighting(commandBuffer);
	CmdDrawFrameDeferredLighting(commandBuffer);
	CmdEndRendering(commandBuffer);
	// - Make color target readable
	CmdBarrierColorWriteToRead(commandBuffer);

	// Post-process
	// - Make post-process image writable
	CmdBarrierPostProcessNoneToWrite(commandBuffer);
	// - Write post-process image
	CmdBeginRenderingPostProcess(commandBuffer);
	CmdDrawFramePostProcess(commandBuffer);
	CmdEndRendering(commandBuffer);
	// - Make post-process image readable
	CmdBarrierPostProcessWriteToRead(commandBuffer);
}

void USceneRender::CreateSampler()
{
	static auto const* sampler{ SAssetManager<ASampler>::Get("${ENGINE_DIRECTORY}/Graphics/assets/samplers/default.kasset") };
	if (sampler)
	{
		samplerIndex_ = sampler->GetIndex();
	}
	else
	{
		throw "failed to load sampler!";
	}
}

void USceneRender::CreateImageResources(VkFormat swapChainFormat)
{
	auto const [width, height] { extent_ };
	Context.CreateImageAndImageView(colorTarget_,		UAllocatedImageCreateInfo::CreateSampled2D(width, height, 1, VK_FORMAT_R16G16B16A16_SFLOAT,	VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,			VK_IMAGE_ASPECT_COLOR_BIT));
	Context.CreateImageAndImageView(albedoTarget_,		UAllocatedImageCreateInfo::CreateSampled2D(width, height, 1, VK_FORMAT_R8G8B8A8_SRGB,		VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,			VK_IMAGE_ASPECT_COLOR_BIT));
	Context.CreateImageAndImageView(normalTarget_,		UAllocatedImageCreateInfo::CreateSampled2D(width, height, 1, VK_FORMAT_R16G16B16A16_SFLOAT,	VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,			VK_IMAGE_ASPECT_COLOR_BIT));
	Context.CreateImageAndImageView(ormTarget_,			UAllocatedImageCreateInfo::CreateSampled2D(width, height, 1, VK_FORMAT_R8G8B8A8_UNORM,		VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,			VK_IMAGE_ASPECT_COLOR_BIT));
	Context.CreateImageAndImageView(depthTarget_,		UAllocatedImageCreateInfo::CreateSampled2D(width, height, 1, Context.GetDepthFormat(),		VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,	VK_IMAGE_ASPECT_DEPTH_BIT));
	Context.CreateImageAndImageView(postProcessTarget_,	UAllocatedImageCreateInfo::CreateSampled2D(width, height, 1, swapChainFormat,				VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,			VK_IMAGE_ASPECT_COLOR_BIT));
}

void USceneRender::CleanupImageResources() const
{
	vkDestroyImageView(Context.GetDevice(), depthTarget_.imageView, nullptr);
	vmaDestroyImage(Context.GetAllocator(), depthTarget_.image, depthTarget_.allocation);
	vkDestroyImageView(Context.GetDevice(), albedoTarget_.imageView, nullptr);
	vmaDestroyImage(Context.GetAllocator(), albedoTarget_.image, albedoTarget_.allocation);
	vkDestroyImageView(Context.GetDevice(), normalTarget_.imageView, nullptr);
	vmaDestroyImage(Context.GetAllocator(), normalTarget_.image, normalTarget_.allocation);
	vkDestroyImageView(Context.GetDevice(), ormTarget_.imageView, nullptr);
	vmaDestroyImage(Context.GetAllocator(), ormTarget_.image, ormTarget_.allocation);

	vkDestroyImageView(Context.GetDevice(), colorTarget_.imageView, nullptr);
	vmaDestroyImage(Context.GetAllocator(), colorTarget_.image, colorTarget_.allocation);
	vkDestroyImageView(Context.GetDevice(), postProcessTarget_.imageView, nullptr);
	vmaDestroyImage(Context.GetAllocator(), postProcessTarget_.image, postProcessTarget_.allocation);
}

void USceneRender::RegisterFrameContextBufferTextures()
{
	depthIndex_ = PipelineResourceManager.RegisterTexture(depthTarget_.imageView);
	albedoIndex_ = PipelineResourceManager.RegisterTexture(albedoTarget_.imageView);
	normalIndex_ = PipelineResourceManager.RegisterTexture(normalTarget_.imageView);
	ormIndex_ = PipelineResourceManager.RegisterTexture(ormTarget_.imageView);
	colorIndex_ = PipelineResourceManager.RegisterTexture(colorTarget_.imageView);
	postProcessIndex_ = PipelineResourceManager.RegisterTexture(postProcessTarget_.imageView);
}

void USceneRender::UnregisterFrameContextBufferTextures() const
{
	PipelineResourceManager.UnregisterTexture(depthIndex_);
	PipelineResourceManager.UnregisterTexture(albedoIndex_);
	PipelineResourceManager.UnregisterTexture(normalIndex_);
	PipelineResourceManager.UnregisterTexture(ormIndex_);
	PipelineResourceManager.UnregisterTexture(colorIndex_);
	PipelineResourceManager.UnregisterTexture(postProcessIndex_);
}

UFrameContextAddresses USceneRender::MakeFrameContextAddresses() const
{
	return {
		.drawDataBufferAddress = drawDataBuffer_.GetAddress(),
		.materialBufferAddress = MaterialBuffer.GetAddress(),
		.transformBufferAddress = transformBuffer_.GetAddress(),
		.parametersBufferAddress = parametersBuffer_.GetAddress(),

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
				.materialIndex = drawCommand.materialIndex,
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

void USceneRender::CmdUpdateClusterAABB(VkCommandBuffer commandBuffer) const
{
	static auto const* shader{ SAssetManager<AShader>::Get("${ENGINE_DIRECTORY}/Graphics/assets/shaders/clusterAABB.kasset") };
	if (shader)
	{
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, shader->GetPipeline());
		CmdPushConstants(commandBuffer, 0, 0);
		vkCmdDispatch(commandBuffer, 16, 9, 24); // todo: variable
	}
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

void USceneRender::CmdDispatchLightBinning(VkCommandBuffer commandBuffer) const
{
	static auto const* shader{ SAssetManager<AShader>::Get("${ENGINE_DIRECTORY}/Graphics/assets/shaders/lightBinning.kasset") };
	if (shader)
	{
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, shader->GetPipeline());
		CmdPushConstants(commandBuffer, 0, 0);
		vkCmdDispatch(commandBuffer, 16, 9, 24);
	}
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

void USceneRender::CmdDrawFrameShadowMaps(VkCommandBuffer commandBuffer, std::span<UDrawCommand const> drawCommands, u32 directionalLightCount) const
{
	static auto const* shader{ SAssetManager<AShader>::Get("${ENGINE_DIRECTORY}/Graphics/assets/shaders/shadowPrePass.kasset") };
	if (!shader)
	{
		KT_LOG(KT_LOG_COMPILE_TIME_LEVEL, "Graphics", "couldn't load shader {0}", "${ENGINE_DIRECTORY}/Graphics/assets/shaders/shadowPrePass.kasset");
		return;
	}

	IndexBuffer.CmdBind(commandBuffer);

	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, shader->GetPipeline());

	for (u32 i{ 0 }; i < directionalLightCount; ++i)
	{
		lightBuffers_.CmdBeginRenderingShadowMapTarget(commandBuffer, i);
		// todo: add cast shadow check
		CmdDrawFrame(commandBuffer, drawCommands, i);

		CmdEndRendering(commandBuffer);
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

void USceneRender::CmdDrawFrameDepthPrePass(VkCommandBuffer commandBuffer, std::span<UDrawCommand const> drawCommands) const
{
	static auto const* shader{ SAssetManager<AShader>::Get("${ENGINE_DIRECTORY}/Graphics/assets/shaders/depthPrePass.kasset") };
	if (shader)
	{
		IndexBuffer.CmdBind(commandBuffer);

		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, shader->GetPipeline());

		CmdDrawFrame(commandBuffer, drawCommands, 0);
	}
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

void USceneRender::CmdDrawFrameGBuffer(VkCommandBuffer commandBuffer, std::span<UDrawCommand const> drawCommands) const
{
	std::unordered_map<VkPipeline, std::vector<UDrawCommand>> pipelineDrawCommands{};

	for (auto const& drawCommand : drawCommands)
	{
		pipelineDrawCommands[drawCommand.pipeline].push_back(drawCommand);
	}

	IndexBuffer.CmdBind(commandBuffer);

	for (auto const& [pipeline, drawCommands] : pipelineDrawCommands)
	{
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);

		CmdDrawFrame(commandBuffer, drawCommands, 0);
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

void USceneRender::CmdDrawFrameDeferredLighting(VkCommandBuffer commandBuffer) const
{
	auto const* shader{ SAssetManager<AShader>::Get("${ENGINE_DIRECTORY}/Graphics/assets/shaders/deferredLighting.kasset") };
	if (shader)
	{
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, shader->GetPipeline());
		CmdPushConstants(commandBuffer, 0, 0);
		vkCmdDraw(commandBuffer, 3, 1, 0, 0);
	}
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

void USceneRender::CmdDrawFramePostProcess(VkCommandBuffer commandBuffer) const
{
	static auto const* shader{ SAssetManager<AShader>::Get("${ENGINE_DIRECTORY}/Graphics/assets/shaders/postProcess.kasset") };
	if (shader)
	{
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, shader->GetPipeline());
		CmdPushConstants(commandBuffer, 0, 0);
		vkCmdDraw(commandBuffer, 3, 1, 0, 0);
	}
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
		, PipelineResourceManager.GetPipelineLayout()
		, VK_SHADER_STAGE_ALL
		, 0
		, sizeof(UPushConstants)
		, &pc
	);
}
