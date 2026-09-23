#include "DefaultSceneContext.h"

#include "GameStateButton.h"
#include "SceneExplorer.h"
#include "ViewController.h"
#include <kotono_interface/widgets.h>

WidgetPtr WDefaultSceneContext::Build()
{
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
				UCreate<WConstraint>{ "Scene Explorer Constraint" }(EAxis::Horizontal, 300.0f)
				| Apply(&WConstraint::SetAxis, EAxis::Horizontal)
				| Apply(&WConstraint::SetSize, 300.0f)
				| (
					UCreate<WSceneExplorer>{ "Scene Explorer" }(GetScene())
				)
			)
			| (
				UCreate<WViewController>{ "Scene View Controller" }(GetScene())
			)
			| (
				UCreate<WViewController>{ "Scene View Controller" }(GetScene())
			)
		)
	);
}

#include "generated/DefaultSceneContext.generated.inl"
