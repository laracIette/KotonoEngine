#include "Renderer.h"

#include "Barriers.h"
#include "DirectionalLight.h"
#include "DrawCommand.h"
#include "IndexBuffer.h"
#include "Material.h"
#include "Model.h"
#include "PipelineResourceManager.h"
#include "PointLight.h"
#include "Sampler.h"
#include "Shader.h"
#include "Texture.h"
#include <assert.h>
#include <kotono_common/log.h>
#include <kotono_graphics/InterfaceRenderGraph.h>
#include <kotono_graphics/SceneRenderGraph.h>
#include <kotono_graphics/SceneView.h>
#include <kotono_platform/Device.h>
#include <kotono_platform/glm_utils.h>
#include <kotono_platform/Swapchain.h>
#include <kotono_platform/vk_utils.h>
#include <ranges>

static constexpr bool IS_MULTI_THREADED{ false };

static void JoinThread(std::thread& thread)
{
	if (thread.joinable())
	{
		thread.join();
	}
}

template <std::derived_from<AAsset> T>
struct GetOrCreateResult
{
	b8 exists;
	T* value;
};

template <std::derived_from<AAsset> T>
static GetOrCreateResult<T> GetOrCreate(UPath const& path, std::unordered_map<UPath, T*>& registry)
{
	auto const it{ registry.find(path) };
	if (it != registry.end())
	{
		return { true, it->second };
	}

	assert(path.IsFile());

	T* texture{ new T{ path } };
	registry[path] = texture;
	return { false, texture };
}

URenderer::URenderer(UDevice& device, USurface& surface)
	: device_{ device }
	, swapchain_{ device, surface }
	, pipelineResourceManager_{ device }
	, sceneRenderer_{ device, swapchain_, pipelineResourceManager_ }
	, interfaceRenderer_{ device, pipelineResourceManager_ }
	, indexBuffer_{ device }
{
}

void URenderer::Init()
{
	swapchain_.Init();

	CreateCommandPools();
	CreateCommandBuffers();
	CreateSyncObjects();

	pipelineResourceManager_.Init();
	indexBuffer_.Init();

	interfaceRenderer_.Init();

	InitSceneRendererResources();
}

void URenderer::Cleanup()
{
	KT_LOG(ELogImportanceLevel::High, "Graphics", "cleaning up renderer");

	JoinThread(renderThread_);
	JoinThread(rhiThread_);

	for (auto const* texture : textures_ | std::views::values)
	{
		texture->Cleanup(device_);
		delete texture;
	}
	for (auto const* material : materials_ | std::views::values)
	{
		delete material;
	}
	for (auto const* sampler : samplers_ | std::views::values)
	{
		sampler->Cleanup(device_);
		delete sampler;
	}
	for (auto const* model : models_ | std::views::values)
	{
		model->Cleanup(device_);
		delete model;
	}
	for (auto const* shader : shaders_ | std::views::values)
	{
		shader->Cleanup(device_);
		delete shader;
	}

	sceneRenderer_.Cleanup();
	interfaceRenderer_.Cleanup();

	indexBuffer_.Cleanup();
	pipelineResourceManager_.Cleanup();

	swapchain_.Cleanup();

	for (const auto& frameData : frameDatas_)
	{
		vkDestroySemaphore(device_.GetDevice(), frameData.renderFinishedSemaphore, nullptr);
		vkDestroySemaphore(device_.GetDevice(), frameData.imageAvailableSemaphore, nullptr);
		vkDestroyFence(device_.GetDevice(), frameData.inFlightFence, nullptr);
		vkDestroyCommandPool(device_.GetDevice(), frameData.commandPool, nullptr);
	}

	KT_LOG(ELogImportanceLevel::High, "Graphics", "cleaned up renderer");
}

void URenderer::DrawFrame(USceneRenderGraph const& sceneRenderGraph, UInterfaceRenderGraph const& interfaceRenderGraph)
{
	u32 const frameIndex{ GetGameThreadFrame() };

	sceneRenderer_.RefreshAvailableSceneRenders(frameIndex);

	auto const interfaceDrawCommands{ MakeInterfaceDrawCommands(interfaceRenderGraph.drawDatas, frameIndex) };

	auto const sceneDrawCommands{ MakeSceneDrawCommands(sceneRenderGraph.drawDatas, frameIndex) };
	auto const pointLights{ MakePointLights(sceneRenderGraph.pointLightDatas) };

	sceneRenderer_.RefreshAvailableSceneRenders(frameIndex);

	auto const sceneRenderViews{ MakeSceneRenderViews(interfaceRenderGraph.drawDatas, frameIndex) };

	sceneRenderer_.ClearUnusedSceneRenders(frameIndex);

	for (auto const& [sceneRender, sceneView] : sceneRenderViews)
	{
		auto const directionalLights{ MakeDirectionalLights(sceneRenderGraph.directionalLightDatas, sceneView, sceneRender, frameIndex) };

		sceneRenderer_.UpdateSceneBuffers(
			  frameIndex
			, sceneRender
			, sceneView
			, sceneDrawCommands
			, directionalLights
			, pointLights
			, defaultSampler_
		);
	}

	interfaceRenderer_.UpdateInterfaceBuffers(interfaceDrawCommands, frameIndex);

	if constexpr (IS_MULTI_THREADED)
	{
		if (frameCount_ >= 1)
		{
			JoinThread(renderThread_);
			u32 const renderThreadFrame{ GetRenderThreadFrame() };
			renderThread_ = std::thread{ &URenderer::RecordCommandBuffer, this, renderThreadFrame };
		}

		if (frameCount_ >= 2)
		{
			KT_LOG(ELogImportanceLevel::High, "Graphics", "frame {0} rendered", frameCount_);

			JoinThread(rhiThread_);
			device_.ExecuteSingleTimeCommands();
			u32 const renderRHIFrame{ GetRHIThreadFrame() };
			rhiThread_ = std::thread{ &URenderer::SubmitCommandBuffer, this, renderRHIFrame };
		}
	}
	else
	{
		if (!TryAcquireNextImage(frameIndex))
		{
			KT_LOG(ELogImportanceLevel::High, "Graphics", "frame {0} skipped", frameCount_);
			return;
		}

		RecordCommandBuffer(
			  frameIndex
			, sceneRenderViews
			, sceneDrawCommands
			, interfaceDrawCommands
			, sceneRenderGraph.directionalLightDatas.size()
		);

		device_.ExecuteSingleTimeCommands();

		SubmitCommandBuffer(frameIndex);
	}

	frameCount_++;
}

void URenderer::InitSceneRendererResources()
{
	defaultSampler_ = GetOrCreateSampler("${ENGINE_DIRECTORY}/Graphics/assets/samplers/default.kasset")->GetIndex();

	clusterAABBPipeline_ = GetOrCreateShader("${ENGINE_DIRECTORY}/Graphics/assets/shaders/clusterAABB.kasset")->GetPipeline();
	lightBinningPipeline_ = GetOrCreateShader("${ENGINE_DIRECTORY}/Graphics/assets/shaders/lightBinning.kasset")->GetPipeline();
	shadowPrePassPipeline_ = GetOrCreateShader("${ENGINE_DIRECTORY}/Graphics/assets/shaders/shadowPrePass.kasset")->GetPipeline();
	depthPrePassPipeline_ = GetOrCreateShader("${ENGINE_DIRECTORY}/Graphics/assets/shaders/depthPrePass.kasset")->GetPipeline();
	deferredLightingPipeline_ = GetOrCreateShader("${ENGINE_DIRECTORY}/Graphics/assets/shaders/deferredLighting.kasset")->GetPipeline();
	postProcessPipeline_ = GetOrCreateShader("${ENGINE_DIRECTORY}/Graphics/assets/shaders/postProcess.kasset")->GetPipeline();
}

void URenderer::RecreateFrames()
{
	// Wait for CPU
	JoinThread(renderThread_);
	JoinThread(rhiThread_);

	// Wait for GPU
	vkDeviceWaitIdle(device_.GetDevice());

	swapchain_.Cleanup();
	swapchain_.Init();

	for (auto const& frameData : frameDatas_)
	{
		vkResetCommandPool(device_.GetDevice(), frameData.commandPool, 0);
	}
}

bool URenderer::TryAcquireNextImage(u32 frameIndex)
{
	// Wait for current frame to be rendered
	VK_CHECK_THROW(
		vkWaitForFences(device_.GetDevice(), 1, &frameDatas_[frameIndex].inFlightFence, VK_TRUE, UINT64_MAX),
		"failed to wait for fences!"
	);

	// Set image index for current frame
	VkResult const result{ swapchain_.AcquireNextImage(
		  UINT64_MAX
		, frameDatas_[frameIndex].imageAvailableSemaphore
		, VK_NULL_HANDLE
		, frameDatas_[frameIndex].imageIndex
	) };
	if (result == VK_ERROR_OUT_OF_DATE_KHR)
	{
		RecreateFrames();
		return false;
	}
	else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
	{
		throw "failed to acquire swap chain image!";
	}

	vkResetFences(device_.GetDevice(), 1, &frameDatas_[frameIndex].inFlightFence);

	return true;
}

void URenderer::CreateCommandPools()
{
	for (size i{ 0 }; i < KT_FRAMES_IN_FLIGHT; ++i)
	{
		CreateCommandPool(static_cast<u32>(i));
	}
}

void URenderer::CreateCommandPool(u32 frameIndex)
{
	UQueueFamilyIndices const queueFamilyIndices{ device_.GetQueueFamilyIndices() };

	const VkCommandPoolCreateInfo poolInfo{
		.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
		.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
		.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value(),
	};
	VK_CHECK_THROW(
		vkCreateCommandPool(device_.GetDevice(), &poolInfo, nullptr, &frameDatas_[frameIndex].commandPool),
		"failed to create command pool!"
	);
}

void URenderer::CreateCommandBuffers()
{
	for (size i{ 0 }; i < KT_FRAMES_IN_FLIGHT; ++i)
	{
		CreateCommandBuffer(static_cast<u32>(i));
	}
}

void URenderer::CreateCommandBuffer(u32 frameIndex)
{
	VkCommandBufferAllocateInfo const allocInfo{
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
		.commandPool = frameDatas_[frameIndex].commandPool,
		.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
		.commandBufferCount = 1,
	};
	VK_CHECK_THROW(
		vkAllocateCommandBuffers(device_.GetDevice(), &allocInfo, &frameDatas_[frameIndex].commandBuffer),
		"failed to allocate command buffers!"
	);
}

void URenderer::CreateSyncObjects()
{
	VkSemaphoreCreateInfo const semaphoreInfo{
		.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
	};
	VkFenceCreateInfo const fenceInfo{
		.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
		.flags = VK_FENCE_CREATE_SIGNALED_BIT,
	};
	for (auto& frameData : frameDatas_)
	{
		if (vkCreateSemaphore(device_.GetDevice(), &semaphoreInfo, nullptr, &frameData.imageAvailableSemaphore) != VK_SUCCESS ||
			vkCreateSemaphore(device_.GetDevice(), &semaphoreInfo, nullptr, &frameData.renderFinishedSemaphore) != VK_SUCCESS ||
			vkCreateFence(device_.GetDevice(), &fenceInfo, nullptr, &frameData.inFlightFence) != VK_SUCCESS)
		{
			throw std::runtime_error{ "failed to create synchronization objects for a frame!" };
		}
	}
}

void URenderer::RecordCommandBuffer(
	  u32 frameIndex
	, std::span<SceneRenderView const> sceneRenderViews
	, std::span<UDrawCommand const> sceneDrawCommands
	, std::span<UDrawCommand const> interfaceDrawCommands
	, u32 directionalLightCount
) const
{
	VkCommandBuffer commandBuffer{ frameDatas_[frameIndex].commandBuffer };
	vkResetCommandBuffer(commandBuffer, 0);

	BeginCommandBuffer(commandBuffer);

	pipelineResourceManager_.CmdBindDescriptorSet(commandBuffer);

	// Scene
	for (auto const handle : sceneRenderViews | std::views::transform(&SceneRenderView::sceneRender))
	{
		sceneRenderer_.CmdDrawScene(frameIndex, handle
			, {
				.commandBuffer = commandBuffer,
				.clusterAABBPipeline = clusterAABBPipeline_,
				.lightBinningPipeline = lightBinningPipeline_,
				.shadowPrePassPipeline = shadowPrePassPipeline_,
				.depthPrePassPipeline = depthPrePassPipeline_,
				.deferredLightingPipeline = deferredLightingPipeline_,
				.postProcessPipeline = postProcessPipeline_,
				.indexBuffer = indexBuffer_,
			}
			, {
				.drawCommands = sceneDrawCommands,
				.directionalLightCount = directionalLightCount,
			}
		);
	}

	auto const [width, height] { swapchain_.GetExtent() };
	VkViewport const viewport{
		.x = 0.0f,
		.y = 0.0f,
		.width = static_cast<f32>(width),
		.height = static_cast<f32>(height),
		.minDepth = 0.0f,
		.maxDepth = 1.0f,
	};
	vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

	// Interface
	CmdBarrierSwapchainNoneToWrite(commandBuffer, frameIndex);
	// - Write swapchain image
	CmdBeginRenderingInterface(commandBuffer, frameIndex);
	interfaceRenderer_.CmdDrawInterface(commandBuffer
		, frameIndex
		, interfaceDrawCommands
		, indexBuffer_
	);
	CmdEndRendering(commandBuffer);
	// - Make swapchain image presentable
	CmdBarrierSwapchainWriteToPresent(commandBuffer, frameIndex);

	EndCommandBuffer(commandBuffer);
}

void URenderer::BeginCommandBuffer(VkCommandBuffer commandBuffer) const
{
	VkCommandBufferBeginInfo const beginInfo{
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
	};
	VK_CHECK_THROW(
		vkBeginCommandBuffer(commandBuffer, &beginInfo),
		"failed to begin recording command buffer!"
	);
}

void URenderer::CmdBarrierSwapchainNoneToWrite(VkCommandBuffer commandBuffer, u32 frameIndex) const
{
	Barriers::CmdTransitionImages(commandBuffer
		, std::array{ swapchain_.GetAllocatedImage(frameDatas_[frameIndex].imageIndex).image }
		, VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT
		, VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT
		, VK_ACCESS_2_NONE
		, VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT
		, VK_IMAGE_LAYOUT_UNDEFINED
		, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
		, { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 }
	);
}

void URenderer::CmdBeginRenderingInterface(VkCommandBuffer commandBuffer, u32 frameIndex) const
{
	const VkRenderingAttachmentInfo swapchainAttachment{
		.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
		.imageView = swapchain_.GetAllocatedImage(frameDatas_[frameIndex].imageIndex).imageView,
		.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,

		.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
		.storeOp = VK_ATTACHMENT_STORE_OP_STORE,
		.clearValue{
			.color = { 0.0f, 0.0f, 0.0f, 1.0f },
		},
	};

	const VkRenderingInfo swapchainRenderingInfo{
		.sType = VK_STRUCTURE_TYPE_RENDERING_INFO,
		.renderArea{
			.offset = { 0, 0 },
			.extent = swapchain_.GetExtent()
		},
		.layerCount = 1,
		.colorAttachmentCount = 1,
		.pColorAttachments = &swapchainAttachment,
	};

	vkCmdBeginRendering(commandBuffer, &swapchainRenderingInfo);
}

void URenderer::CmdBarrierSwapchainWriteToPresent(VkCommandBuffer commandBuffer, u32 frameIndex) const
{
	Barriers::CmdTransitionImages(commandBuffer
		, std::array{ swapchain_.GetAllocatedImage(frameDatas_[frameIndex].imageIndex).image }
		, VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT
		, VK_PIPELINE_STAGE_2_NONE
		, VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT
		, VK_ACCESS_2_NONE
		, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
		, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
		, { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 }
	);
}

void URenderer::CmdEndRendering(VkCommandBuffer commandBuffer) const
{
	vkCmdEndRendering(commandBuffer);
}

void URenderer::EndCommandBuffer(VkCommandBuffer commandBuffer) const
{
	VK_CHECK_THROW(
		vkEndCommandBuffer(commandBuffer),
		"failed to record command buffer!"
	);
}

void URenderer::SubmitCommandBuffer(u32 frameIndex)
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
		vkQueueSubmit2(device_.GetGraphicsQueue(), 1, &submitInfo2, frameDatas_[frameIndex].inFlightFence),
		"failed to submit draw command buffer!"
	);

	VkResult const result{ swapchain_.QueuePresent(
		  frameDatas_[frameIndex].renderFinishedSemaphore
		, frameDatas_[frameIndex].imageIndex
	) };

	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
	{
		RecreateFrames();
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

u32 URenderer::GetGameThreadFrame() const
{
	// Prepare game thread for render thread
	return frameCount_ % static_cast<u32>(KT_FRAMES_IN_FLIGHT);
}

u32 URenderer::GetRenderThreadFrame() const
{
	// Prepare render thread for RHI thread
	return ((frameCount_ + KT_FRAMES_IN_FLIGHT) - 1) % static_cast<u32>(KT_FRAMES_IN_FLIGHT); // avoid negative with + KT_FRAMES_IN_FLIGHT
}

u32 URenderer::GetRHIThreadFrame() const
{
	// Prepare RHI thread for game thread
	return ((frameCount_ + KT_FRAMES_IN_FLIGHT) - 2) % static_cast<u32>(KT_FRAMES_IN_FLIGHT); // avoid negative with + KT_FRAMES_IN_FLIGHT
}

UFrameContextSceneView URenderer::MakeFrameContextSceneView(USceneView const& sceneView) const
{
	return {
		.view = sceneView.view,
		.proj = sceneView.proj,
		.viewPos = sceneView.viewPos,
		.extent = sceneView.extent,
		.time = sceneView.time,
		.fov = sceneView.fov,
		.aspectRatio = sceneView.aspectRatio,
		.depthNear = sceneView.depthNear,
	};
}

std::vector<UDrawCommand> URenderer::MakeInterfaceDrawCommands(std::span<UDrawData const> drawDatas, u32 frameIndex)
{
	return drawDatas
		| std::views::filter(&UDrawData::isVisible)
		| std::views::enumerate
		| std::views::transform([this, frameIndex](auto&& tuple) {
			auto const& [index, drawData] { tuple };

			auto const* shader{ GetOrCreateShader(drawData.shader) };
			auto const* model{ GetOrCreateModel(drawData.model) };

			std::array<f32, 16> scalars{};
			std::array<glm::vec4, 16> vectors{};
			std::array<u32, 16> textures{};

			std::ranges::copy(drawData.scalars | std::views::take(16), scalars.begin());
			std::ranges::copy(drawData.vectors | std::views::take(16), vectors.begin());
			std::ranges::copy(drawData.textures | std::views::take(16)
				| std::views::transform([this, frameIndex](UDrawData::Texture const& texture) {
					return GetTextureHandle(texture, frameIndex);
				})
				, textures.begin()
			);

			return UDrawCommand{
				.drawIndex = static_cast<u32>(index),
				.pipeline = shader->GetPipeline(),
				.vertexBufferAddress = model->GetVertexBufferAddress(),
				.indexCount = model->GetIndexCount(),
				.firstIndex = model->GetFirstIndex(),
				.scissor{
					.offset = { drawData.scissor.offset.x, drawData.scissor.offset.y },
					.extent = { drawData.scissor.extent.x, drawData.scissor.extent.y },
				},
				.modelMatrix = drawData.modelMatrix,
				.scalars = scalars,
				.vectors = vectors,
				.textures = textures,
			};
		})
		| std::ranges::to<std::vector>();
}

std::vector<UDrawCommand> URenderer::MakeSceneDrawCommands(std::span<UDrawData const> drawDatas, u32 frameIndex)
{
	return drawDatas
		| std::views::filter(&UDrawData::isVisible)
		| std::views::enumerate
		| std::views::transform([this, frameIndex](auto&& tuple) {
			auto const& [index, drawData] { tuple };

			auto const* shader{ GetOrCreateShader(drawData.shader) };
			auto const* model{ GetOrCreateModel(drawData.model) };
			auto const* material{ GetOrCreateMaterial(drawData.material) };
			
			auto const materialData{ material->GetData() };

			std::array<f32, 16> scalars{};
			std::array<glm::vec4, 16> vectors{};
			std::array<u32, 16> textures{};

			std::ranges::copy(drawData.scalars | std::views::take(16), scalars.begin());
			std::ranges::copy(drawData.vectors | std::views::take(16), vectors.begin());
			std::ranges::copy(drawData.textures | std::views::take(16)
				| std::views::transform([this, frameIndex](UDrawData::Texture const& texture) {
					return GetTextureHandle(texture, frameIndex);
				})
				, textures.begin()
			);

			return UDrawCommand{
				.drawIndex = static_cast<u32>(index),
				.pipeline = shader->GetPipeline(),
				.vertexBufferAddress = model->GetVertexBufferAddress(),
				.indexCount = model->GetIndexCount(),
				.firstIndex = model->GetFirstIndex(),
				.material = {
					.albedoIndex = GetOrCreateTexture(materialData.albedo)->GetIndex(),
					.normalIndex = GetOrCreateTexture(materialData.normal)->GetIndex(),
					.ormIndex = GetOrCreateTexture(materialData.orm)->GetIndex(),
					.emissiveIndex = GetOrCreateTexture(materialData.emissive)->GetIndex(),
					.materialType = materialData.materialType,
					.samplerIndex = GetOrCreateSampler(materialData.sampler)->GetIndex(),
				},
				.modelMatrix = drawData.modelMatrix,
				.normalMatrix = drawData.normalMatrix,
				.sortKey = drawData.sortKey,
				.scalars = scalars,
				.vectors = vectors,
				.textures = textures,
			};
		})
		| std::ranges::to<std::vector>();
}

std::vector<UDirectionalLight> URenderer::MakeDirectionalLights(
	  std::span<UDirectionalLightData const> directionalLightDatas
	, UFrameContextSceneView const& sceneView
	, u32 sceneRender
	, u32 frameIndex
)
{
	static auto* sampler{ GetOrCreateSampler("${ENGINE_DIRECTORY}/Graphics/assets/samplers/shadow.kasset") };

	std::array<f32, NUM_DIRECTIONAL_CASCADES + 1> const cascadeSplits{
		sceneView.depthNear,
		5.0f, 15.0f, 50.0f, 200.0f,
	};

	auto const makeLightViewProjs{ [sceneView, cascadeSplits](glm::vec3 const& direction) {
		std::array<glm::mat4, NUM_DIRECTIONAL_CASCADES> lightViewProjs{};
		
		std::ranges::copy(std::views::iota(0u, NUM_DIRECTIONAL_CASCADES)
			| std::views::transform([direction, sceneView, cascadeSplits](u32 index) {
				return get_light_space_matrix(
					  direction
					, sceneView.view
					, cascadeSplits[index]
					, cascadeSplits[index + 1]
					, sceneView.fov
					, sceneView.aspectRatio
				);
			})
			, lightViewProjs.begin()
		);

		return lightViewProjs;
	} };

	return directionalLightDatas
		| std::views::enumerate
		| std::views::transform([this, makeLightViewProjs, sceneRender, frameIndex](auto&& tuple) {
			auto const& [index, directionalLightData] { tuple };

			return UDirectionalLight{
				.direction = directionalLightData.direction,
				.color = directionalLightData.color,
				.intensity = directionalLightData.intensity,
				.castShadow = static_cast<u32>(directionalLightData.castShadow),
				.shadowMap = sceneRenderer_.GetSceneDirectionalLightShadowMapTargetIndex(frameIndex, sceneRender, index),
				.shadowSampler = sampler->GetIndex(),
				.lightViewProjs = makeLightViewProjs(directionalLightData.direction),
				.cascadeSplits = { 5.0f, 15.0f, 50.0f, 200.0f },
			};
		})
		| std::ranges::to<std::vector>();
}

std::vector<UPointLight> URenderer::MakePointLights(std::span<UPointLightData const> pointLightDatas) const
{
	return pointLightDatas
		| std::views::transform([](UPointLightData const& pointLightData) {
			return UPointLight{
				.position = pointLightData.position,
				.range = pointLightData.range,
				.color = pointLightData.color,
				.intensity = pointLightData.intensity,
			};
		})
		| std::ranges::to<std::vector>();
}

std::vector<URenderer::SceneRenderView> URenderer::MakeSceneRenderViews(std::span<UDrawData const> drawDatas, u32 frameIndex)
{
	return drawDatas
		| std::views::transform(&UDrawData::textures)
		| std::views::join
		| std::views::filter([](UDrawData::Texture const& texture) {
			return std::holds_alternative<USceneView>(texture);
		})
		| std::views::transform([this, frameIndex](UDrawData::Texture const& texture) {
			auto const& sceneView{ std::get<USceneView>(texture) };
			return SceneRenderView{
				.sceneRender = sceneRenderer_.GetSceneRender(sceneView.extent, frameIndex),
				.sceneView = MakeFrameContextSceneView(sceneView),
			};
		})
		| std::ranges::to<std::vector>();
}

ATexture* URenderer::GetOrCreateTexture(UPath const& path)
{
	auto const [exists, texture] { GetOrCreate(path, textures_) };
	if (!exists)
	{
		texture->Init(device_);

		auto const imageView{ texture->GetImageView() };
		texture->SetIndex(pipelineResourceManager_.RegisterTexture(imageView));
	}
	return texture;
}

AMaterial* URenderer::GetOrCreateMaterial(UPath const& path)
{
	auto const [exists, material] { GetOrCreate(path, materials_) };
	if (!exists)
	{
	}
	return material;
}

ASampler* URenderer::GetOrCreateSampler(UPath const& path)
{
	auto const [exists, sampler] { GetOrCreate(path, samplers_) };
	if (!exists)
	{
		sampler->Init(device_);

		auto const samplerType{ sampler->GetType() };
		auto const vkSampler{ sampler->GetSampler() };
		switch (samplerType)
		{
		case ASampler::EType::Sampler:
			sampler->SetIndex(pipelineResourceManager_.RegisterSampler(vkSampler));
			break;
		case ASampler::EType::ShadowSampler:
			sampler->SetIndex(pipelineResourceManager_.RegisterShadowSampler(vkSampler));
			break;
		default:
			break;
		}
	}
	return sampler;
}

AModel* URenderer::GetOrCreateModel(UPath const& path)
{
	auto const [exists, model] { GetOrCreate(path, models_) };
	if (!exists)
	{
		model->Init(device_);

		auto const indices{ model->GetIndices() };
		model->SetFirstIndex(indexBuffer_.RegisterIndices(indices));
	}
	return model;
}

AShader* URenderer::GetOrCreateShader(UPath const& path)
{
	auto const [exists, shader] { GetOrCreate(path, shaders_) };
	if (!exists)
	{
		shader->Init(device_, pipelineResourceManager_.GetPipelineLayout(), swapchain_.GetFormat());
	}
	return shader;
}

u32 URenderer::GetTextureHandle(UDrawData::Texture const& texture, u32 frameIndex)
{
	if (std::holds_alternative<UPath>(texture))
	{
		auto const& path{ std::get<UPath>(texture) };
		return GetOrCreateTexture(path)->GetIndex();
	}
	else
	{
		auto const& sceneView{ std::get<USceneView>(texture) };
		return sceneRenderer_.GetSceneRenderTarget(sceneView.extent, frameIndex);
	}
}
