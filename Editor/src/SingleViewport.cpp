#include "SingleViewport.h"

#include "ViewController.h"
#include "SceneVisibilityWindow.h"
#include <kotono_interface/widgets.h>

static constexpr auto DEFAULT_VISIBILITY{ ESceneVisibility::All };

WidgetPtr WSingleViewport::Build()
{
	UPtr const sceneTexture{ UCreate<WSceneTexture>{ "Scene Texture" }(GetScene()) };

	return (
		UCreate<WStack>{}()
		| (
			sceneTexture
			| Apply(&WSceneTexture::SetSceneVisibility, DEFAULT_VISIBILITY)
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
			UCreate<WSceneVisibilityWindow>{ "Visualizer Window" }(DEFAULT_VISIBILITY)
			| Apply(&WSceneVisibilityWindow::SetOnSceneVisibilityChanged, [sceneTexture](ESceneVisibility visibility) {
				sceneTexture->SetSceneVisibility(visibility);
			})
		)
	);
}

#include "generated/SingleViewport.generated.inl"
