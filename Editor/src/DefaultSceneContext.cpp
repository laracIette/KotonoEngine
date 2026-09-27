#include "DefaultSceneContext.h"

#include "Detachable.h"
#include "GameStateButton.h"
#include "SceneExplorer.h"
#include "SingleViewport.h"
#include <kotono_interface/widgets.h>

WidgetPtr WDefaultSceneContext::Build()
{
	return (
		UCreate<WRow>{}()
		| (
			UCreate<WWrap>{}()
			| Apply(&WWrap::SetAxis, EAxis::Horizontal)
			| (
				UCreate<WColumn>{}()
				| Apply(&WColumn::SetSpacing, 5.0f)
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
					UCreate<WDetachable>{ "Scene Explorer" }()
					| (
						UCreate<WSceneExplorer>{ "Scene Explorer" }(GetScene())
					)
				)
			)
		)
		| (
			UCreate<WDetachable>{ "Game" }()
			| (
				UCreate<WSingleViewport>{}(GetScene())
			)
		)
	);
}

#include "generated/DefaultSceneContext.generated.inl"
