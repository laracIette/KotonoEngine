#include "DefaultSceneContext.h"

#include "Detachable.h"
#include "GameSpeedButton.h"
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
				| (
					UCreate<WWrap>{}()
					| Apply(&WWrap::SetAxis, EAxis::Vertical)
					| (
						UCreate<WDetachable>{ "Game Buttons" }()
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
								UCreate<WWrap>{}()
								| Apply(&WWrap::SetAxis, EAxis::Vertical)
								| (
									UCreate<WAlign>{}()
									| Apply(&WAlign::SetAlignment, UAlignment::Center())
									| (
										UCreate<WGameSpeedButton>{ "Game Speed Button" }(GetScene())
									)
								)
							)
						)
					)
				)
				| (
					UCreate<WDetachable>{ "Scene Explorer" }()
					| (
						UCreate<WSceneExplorer>{}(GetScene())
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
