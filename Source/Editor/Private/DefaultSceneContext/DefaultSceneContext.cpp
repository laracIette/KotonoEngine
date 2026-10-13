#include "DefaultSceneContext.h"

#include "Detachable/Detachable.h"
#include "GameSpeedButton/GameSpeedButton.h"
#include "GameStateButton/GameStateButton.h"
#include "PropertiesWindow/PropertiesWindow.h"
#include "SceneExplorer/SceneExplorer.h"
#include "SingleViewport/SingleViewport.h"
#include "Splitter/HorizontalSplitter.h"
#include "Splitter/VerticalSplitter.h"
#include <core_widgets.h>
#include <Scene/Scene.h>

WidgetPtr WDefaultSceneContext::Build()
{
	return (
		UCreate<WHorizontalSplitter>{}()
		| (
			UCreate<WVerticalSplitter>{}()
			| Apply(&WColumn::SetExpandWeight, glm::vec2{ 0.33f, 1.0f })
			| (
				UCreate<WWrap>{}()
				| Apply(&WWrap::SetAxis, EAxis::Vertical)
				| (
					UCreate<WStack>{}()
					| (
						UCreate<WButton>{}()
						| Apply(&WButton::SetIsEnabled, [this]() { return GetScene()->GetIsGameStopped(); })
						| Apply(&WButton::SetOnClicked, [this]() { GetScene()->Serialize(); })
					)
					| (
						UCreate<WAlign>{}()
						| Apply(&WAlign::SetAlignment, UAlignment::Center())
						| (
							UCreate<WText>{}()
							| Apply(&WText::SetText, "Save Scene")
						)
					)
				)
			)
			| (
				UCreate<WDetachable>{ "Game Buttons" }()
				| Apply(&WDetachable::SetExpandWeight, glm::vec2{ 1.0f, 0.4f })
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
			| (
				UCreate<WDetachable>{ "Scene Explorer" }()
				| (
					UCreate<WSceneExplorer>{}(GetScene())
				)
			)
		)
		| (
			UCreate<WDetachable>{ "Object Properties" }()
			| Apply(&WDetachable::SetExpandWeight, glm::vec2{ 0.33f, 1.0f })
			| (
				UCreate<WPropertiesWindow>{}(GetScene())
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

b8 WDefaultSceneContext::OnMouseButton(EButton button, EInputState inputState, EModifier modifier)
{	
	if (GetScene())
	{
		// Check mesh intersections
		//return INPUT_HANDLED;
	}
	
	return INPUT_UNHANDLED;
}

#include "DefaultSceneContext.generated.inl"
