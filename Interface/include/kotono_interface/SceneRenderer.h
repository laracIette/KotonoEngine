#pragma once
#include "generated/SceneRenderer.generated.h"
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/quaternion_float.hpp>
#include <glm/ext/vector_float3.hpp>
#include <kotono_common/Handle.h>
#include <kotono_common/types.h>
#include <kotono_object/Widget.h>
class WSceneRenderer final : public WWidget
{
	GENERATED_WSCENERENDERER()

public:
	WSceneRenderer();

	void Remove() override;

	void PopulateRenderGraph(UInterfaceRenderGraph& interfaceRenderGraph) const override;

	glm::vec3 GetRightVector() const;
	glm::vec3 GetUpVector() const;
	glm::vec3 GetForwardVector() const;
	glm::mat4 GetViewMatrix() const;
	glm::mat4 GetProjectionMatrix() const;

protected:
	void DisplayInternal(UWidgetDisplaySettings displaySettings) override;

private:
	EHandle renderTarget_;
	StateProperty(glm::vec3, viewPosition_, ViewPosition);
	StateProperty(glm::quat, viewRotation_, ViewRotation);
	StateProperty(f32, verticalFOV_, VerticalFOV);
	StateProperty(f32, depthNear_, DepthNear);
};