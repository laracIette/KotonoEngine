#include "DefaultSceneContext.h"

#include "Detachable.h"
#include "GameStateButton.h"
#include "SceneExplorer.h"
#include "ViewController.h"
#include "VisualizerWindow.h"
#include <kotono_interface/widgets.h>

WidgetPtr WDefaultSceneContext::Build()
{
	UPtr const sceneTexture1{ UCreate<WSceneTexture>{ "Scene Texture" }(GetScene()) };
	UPtr const sceneTexture2{ UCreate<WSceneTexture>{ "Scene Texture" }(GetScene()) };

	return (
		UCreate<WColumn>{}()
		| (
			UCreate<WWrap>{}()
			| Apply(&WWrap::SetAxis, EAxis::Vertical)
			| (
				UCreate<WCenter>{ "Game State Center" }()
				| Apply(&WCenter::SetAxis, EAxis::Horizontal)
				| (
					UCreate<WGameStateButton>{ "Game State Button" }(GetScene())
				)
			)
		)
		| (
			UCreate<WRow>{}()
			| (
				UCreate<WConstraint>{ "Scene Explorer Constraint" }()
				| Apply(&WConstraint::SetAxis, EAxis::Horizontal)
				| Apply(&WConstraint::SetSize, 300.0f)
				| (
					UCreate<WSceneExplorer>{ "Scene Explorer" }(GetScene())
				)
			)
			| (
				UCreate<WDetachable>{ "Game" }()
				| (
					UCreate<WStack>{}()
					| (
						sceneTexture1
					)
					| (
						UCreate<WViewController>{ "Scene View Controller" }(GetScene())
						| Apply(&WViewController::SetOnMove, [sceneTexture1](glm::vec3 const& position) { 
							sceneTexture1->SetViewPosition(position);
						})
						| Apply(&WViewController::SetOnLook, [sceneTexture1](glm::quat const& rotation) { 
							sceneTexture1->SetViewRotation(rotation);
						})
					)
					| (
						UCreate<WVisualizerWindow>{ "Visualizer Window" }()
						| Apply(&WVisualizerWindow::SetOnSceneVisibilityChanged, [sceneTexture1](ESceneVisibility visibility) {
							sceneTexture1->SetSceneVisibility(visibility);
						})
					)
				)
			)
			| (
				UCreate<WDetachable>{ "Game" }()
				| (
					UCreate<WStack>{}()
					| (
						sceneTexture2
					)
					| (
						UCreate<WViewController>{ "Scene View Controller" }(GetScene())
						| Apply(&WViewController::SetOnMove, [sceneTexture2](glm::vec3 const& position) { 
							sceneTexture2->SetViewPosition(position);
						})
						| Apply(&WViewController::SetOnLook, [sceneTexture2](glm::quat const& rotation) { 
							sceneTexture2->SetViewRotation(rotation);
						})
					)
				)
			)
		)
	);
}

#include "generated/DefaultSceneContext.generated.inl"
