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
	, sceneVisibility_{ ESceneVisibility::All }
{
}

void WSceneTexture::PopulateRenderGraph(UInterfaceRenderGraph& interfaceRenderGraph) const
{
	USceneView const sceneView{
		.view = GetViewMatrix(),
		.proj = GetProjectionMatrix(),
		.viewPos = viewPosition_,
		.extent = GetSize(),
		.time = SClock::Now(),
		.fov = GetVerticalFOV(),
		.aspectRatio = GetAspectRatio(),
		.depthNear = GetDepthNear(),
	};

	USceneRenderGraph sceneRenderGraph{};

	if (GetScene())
	{
		GetScene()->PopulateRenderGraph(sceneRenderGraph, sceneVisibility_);
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

auto WSceneTexture::GetViewMatrix() const -> glm::mat4
{
	return glm::lookAt(viewPosition_, viewPosition_ + forward_vector(viewRotation_), up_vector(viewRotation_));
}

auto WSceneTexture::GetProjectionMatrix() const -> glm::mat4
{
	return calculate_reverse_z_infinite_perspective(glm::radians(verticalFOV_), GetAspectRatio(), depthNear_);
}

void WSceneTexture::SetViewPosition(glm::vec3 const& position)
{
	viewPosition_ = position;
	GetScene()->GetAudioContext().SetListenerPosition(position);
}

void WSceneTexture::SetViewRotation(glm::quat const& rotation)
{
	viewRotation_ = rotation;
	GetScene()->GetAudioContext().SetListenerOrientation(rotation);
}

#include "generated/SceneTexture.generated.inl"
