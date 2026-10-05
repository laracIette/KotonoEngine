#include "DefaultSceneContext.h"

#include "Detachable/Detachable.h"
#include "GameSpeedButton/GameSpeedButton.h"
#include "GameStateButton/GameStateButton.h"
#include "PropertiesWindow/PropertiesWindow.h"
#include "SceneExplorer/SceneExplorer.h"
#include "SingleViewport/SingleViewport.h"
#include <core_widgets.h>

WidgetPtr WDefaultSceneContext::Build()
{
	return (
		UCreate<WRow>{}()
		| (
			UCreate<WColumn>{}()
			| Apply(&WColumn::SetExpandWeight, glm::vec2{ 0.33f, 1.0f })
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
		| (
			UCreate<WDetachable>{ "Game" }()
			| (
				UCreate<WSingleViewport>{}(GetScene())
			)
		)
		| (
			UCreate<WDetachable>{ "Object Properties" }()
			| Apply(&WDetachable::SetExpandWeight, glm::vec2{ 0.33f, 1.0f })
			| (
				UCreate<WPropertiesWindow>{}(GetScene())
			)
		)
	);
}

#include "generated/DefaultSceneContext.generated.inl"
