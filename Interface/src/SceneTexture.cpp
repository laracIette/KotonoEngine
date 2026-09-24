#include "SceneTexture.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/quaternion_trigonometric.hpp>
#include <kotono_core/Scene.h>
#include <kotono_graphics/Color.h>
#include <kotono_graphics/InterfaceRenderGraph.h>
#include <kotono_math/math_utils.h>
#include <kotono_timing/Clock.h>

WSceneTexture::WSceneTexture(UScene* scene)
	: Base(scene)
	, depthNear_{ 0.1f }
	, verticalFOV_{ 90.0f }
	, viewPosition_{ WorldUpVector + WorldForwardVector * 2.0f }
	, viewRotation_{ glm::angleAxis(glm::radians(180.0f), WorldUpVector) }
{
}

void WSceneTexture::PopulateRenderGraph(UInterfaceRenderGraph& interfaceRenderGraph) const
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

	USceneRenderGraph sceneRenderGraph{};

	if (GetScene())
	{
		GetScene()->PopulateRenderGraph(sceneRenderGraph);
	}

	interfaceRenderGraph.drawDatas.push_back({
		.scissor = GetScissor(),
		.modelMatrix = GetModelMatrix(),
		.shader = "${ENGINE_DIRECTORY}/Graphics/assets/shaders/shader2D.kasset",
		.model = "${ENGINE_DIRECTORY}/Graphics/assets/models/rectangle.obj",
		.scalars = {},
		.vectors = { Colors::White },
		.textures = { UInterfaceDrawData::SceneRenderData{ sceneView, std::move(sceneRenderGraph) } },
		.isVisible = GetIsVisible(),
	});
}

glm::vec3 WSceneTexture::GetRightVector() const
{
	return viewRotation_ * WorldRightVector;
}

glm::vec3 WSceneTexture::GetUpVector() const
{
	return viewRotation_ * WorldUpVector;
}

glm::vec3 WSceneTexture::GetForwardVector() const
{
	return viewRotation_ * WorldForwardVector;
}

glm::mat4 WSceneTexture::GetViewMatrix() const
{
	return glm::lookAt(viewPosition_, viewPosition_ + GetForwardVector(), GetUpVector());
}

glm::mat4 WSceneTexture::GetProjectionMatrix() const
{
	return calculate_reverse_z_infinite_perspective(glm::radians(verticalFOV_), GetAspectRatio(), depthNear_);
}

#include "generated/SceneTexture.generated.inl"
