#include "DefaultSceneContext.h"

#include "Detachable.h"
#include "GameStateButton.h"
#include "SceneExplorer.h"
#include "SingleViewport.h"
#include <kotono_interface/widgets.h>

WidgetPtr WDefaultSceneContext::Build()
{
	return (
		UCreate<WColumn>{}()
		| (
			UCreate<WWrap>{}()
			| Apply(&WWrap::SetAxis, EAxis::Vertical)
			| (
				UCreate<WAlign>{}()
				| Apply(&WAlign::SetAlignment, UAlignment::Center())
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
					UCreate<WSingleViewport>{}(GetScene())
				)
			)
		)
	);
}

#include "generated/DefaultSceneContext.generated.inl"
