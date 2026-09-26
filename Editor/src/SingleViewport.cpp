#include "SingleViewport.h"

#include "ViewController.h"
#include "VisualizerWindow.h"
#include <kotono_common/enum_utils.h>
#include <kotono_interface/widgets.h>

WidgetPtr WSingleViewport::Build()
{
	UPtr const sceneTexture{ UCreate<WSceneTexture>{ "Scene Texture" }(GetScene()) };

	constexpr auto defaultVisibility{ ESceneVisibility::Mesh | ESceneVisibility::PointLight };

	return (
		UCreate<WStack>{}()
		| (
			sceneTexture
			| Apply(&WSceneTexture::SetSceneVisibility, defaultVisibility)
		)
		| (
			UCreate<WViewController>{ "Scene View Controller" }(GetScene())
			| Apply(&WViewController::SetOnMove, [sceneTexture](glm::vec3 const& position) { 
				sceneTexture->SetViewPosition(position);
			})
			| Apply(&WViewController::SetOnLook, [sceneTexture](glm::quat const& rotation) { 
				sceneTexture->SetViewRotation(rotation);
			})
		)
		| (
			UCreate<WVisualizerWindow>{ "Visualizer Window" }(defaultVisibility)
			| Apply(&WVisualizerWindow::SetOnSceneVisibilityChanged, [sceneTexture](ESceneVisibility visibility) {
				sceneTexture->SetSceneVisibility(visibility);
			})
		)
	);
}

#include "generated/SingleViewport.generated.inl"
