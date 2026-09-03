#pragma once
#include "generated/SceneRenderer.generated.h"
#include <kotono_object/Widget.h>

#include <glm/ext/quaternion_float.hpp>
#include <glm/ext/vector_float3.hpp>
#include <kotono_common/types.h>
class WSceneRenderer final : public WWidget
{
	GENERATED_WSCENERENDERER()

public:
	WSceneRenderer();

	void PopulateRenderGraph(UInterfaceRenderGraph& interfaceRenderGraph) const override;

	glm::vec3 GetRightVector() const;
	glm::vec3 GetUpVector() const;
	glm::vec3 GetForwardVector() const;
	glm::mat4 GetViewMatrix() const;
	glm::mat4 GetProjectionMatrix() const;

private:
	WritableProperty(glm::vec3, viewPosition_, ViewPosition);
	WritableProperty(glm::quat, viewRotation_, ViewRotation);
	WritableProperty(f32, verticalFOV_, VerticalFOV);
	WritableProperty(f32, depthNear_, DepthNear);
};