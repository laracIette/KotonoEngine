#pragma once
#include "generated/SceneTexture.generated.h"
#include <kotono_core/SceneWidget.h>

#include <glm/ext/quaternion_float.hpp>
#include <glm/ext/vector_float3.hpp>
#include <kotono_common/types.h>
#include <kotono_graphics/SceneVisibility.h>

class WSceneTexture final : public WSceneWidget
{
	GENERATED_WSCENETEXTURE()

public:
	WSceneTexture(UScene* scene);

	void PopulateRenderGraph(UInterfaceRenderGraph& interfaceRenderGraph) const override;

	auto GetViewMatrix() const -> glm::mat4;
	auto GetProjectionMatrix() const -> glm::mat4;

	void SetViewPosition(glm::vec3 const& position);
	void SetViewRotation(glm::quat const& rotation);

private:
	glm::vec3 viewPosition_;
	glm::quat viewRotation_;

	WritableProperty(f32, verticalFOV_, VerticalFOV);
	WritableProperty(f32, depthNear_, DepthNear);
	WritableProperty(ESceneVisibility, sceneVisibility_, SceneVisibility, Value);
};