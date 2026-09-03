#include "SceneRenderer.h"

#include <glm/ext/matrix_transform.hpp>
#include <kotono_graphics/Color.h>
#include <kotono_graphics/InterfaceRenderGraph.h>
#include <kotono_platform/glm_utils.h>
#include <kotono_timing/Clock.h>

WSceneRenderer::WSceneRenderer()
	: depthNear_{ 0.1f }
	, verticalFOV_{ 90.0f }
	, viewPosition_{ WorldUpVector - WorldForwardVector }
	, viewRotation_{ glm::identity<glm::quat>() }
{
}

void WSceneRenderer::PopulateRenderGraph(UInterfaceRenderGraph& interfaceRenderGraph) const
{
	USceneView const sceneView{
		.view = GetViewMatrix(),
		.proj = GetProjectionMatrix(),
		.viewPos = GetViewPosition(),
		.extent = GetSize(),
		.time = SClock::Now(),
		.fov = GetVerticalFOV(),
		.aspectRatio = GetAspectRatio(),
		.depthNear = GetDepthNear(),
	};

	interfaceRenderGraph.drawDatas.push_back({
		.scissor = GetScissor(),
		.sortKey = static_cast<f32>(GetLayer()),
		.modelMatrix = ModelMatrix(),
		.normalMatrix = glm::identity<glm::mat4>(),
		.shader = "${ENGINE_DIRECTORY}/Graphics/assets/shaders/shader2D.kasset",
		.material = {},
		.model = "${ENGINE_DIRECTORY}/Graphics/assets/models/rectangle.obj",
		.scalars = {},
		.vectors = { Colors::White },
		.textures = { sceneView },
		.isVisible = true,
	});
}

glm::vec3 WSceneRenderer::GetRightVector() const
{
	return viewRotation_ * WorldRightVector;
}

glm::vec3 WSceneRenderer::GetUpVector() const
{
	return viewRotation_ * WorldUpVector;
}

glm::vec3 WSceneRenderer::GetForwardVector() const
{
	return viewRotation_ * WorldForwardVector;
}

glm::mat4 WSceneRenderer::GetViewMatrix() const
{
	return glm::lookAt(viewPosition_, viewPosition_ + GetForwardVector(), GetUpVector());
}

glm::mat4 WSceneRenderer::GetProjectionMatrix() const
{
	return calculate_reverse_z_infinite_perspective(glm::radians(verticalFOV_), GetAspectRatio(), depthNear_);
}

#include "generated/SceneRenderer.generated.inl"
