#include "DefaultSceneContext.h"

#include "GameStateButton.h"
#include "SceneExplorer.h"
#include "ViewController.h"
#include <kotono_interface/widgets.h>

WidgetPtr WDefaultSceneContext::Build()
{
	auto const widgetTree{ UChildrenOwnerTree{ UCreate<WColumn>{}(), {
		new UChildOwnerTree{ UCreate<WWrap>{}(EAxis::Vertical), 
			new UChildOwnerTree{ UCreate<WCenter>{ "Game State Center" }(EAxis::Horizontal),
				new UWidgetTreeLeaf{ UCreate<WGameStateButton>{ "Game State Button" }(GetScene()) }
			}
		},
		new UChildrenOwnerTree{ UCreate<WRow>{}(), {
			new UChildOwnerTree{ UCreate<WConstraint>{ "Scene Explorer Constraint" }(EAxis::Horizontal, 300.0f),
				new UWidgetTreeLeaf{ UCreate<WSceneExplorer>{ "Scene Explorer" }(GetScene()) }
			},
			new UWidgetTreeLeaf{ UCreate<WViewController>{ "Scene View Controller" }(GetScene()) },
			new UWidgetTreeLeaf{ UCreate<WViewController>{ "Scene View Controller" }(GetScene()) },
		} },
	} } };

	widgetTree.Link();

	return widgetTree.Widget();
}

#include "generated/DefaultSceneContext.generated.inl"
