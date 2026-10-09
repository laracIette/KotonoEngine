#include "SingleViewport.h"

#include "SceneVisibilityWindow/SceneVisibilityWindow.h"
#include "ViewController/ViewController.h"
#include <core_widgets.h>

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
			UCreate<WSwitcher>{}()
			| Apply(&WSwitcher::SetActiveWidget, 1)
			| (
				UCreate<WSceneVisibilityWindow>{ "Visualizer Window" }(DEFAULT_VISIBILITY)
				| Apply(&WSceneVisibilityWindow::SetOnSceneVisibilityChanged, [sceneTexture](ESceneVisibility visibility) {
					sceneTexture->SetSceneVisibility(visibility);
				})
			)
			| (
				UCreate<WText>{}() | Apply(&WText::SetText, "gosfhuiuiofsd")
			)
		)
	);
}

#include "SingleViewport.generated.inl"
