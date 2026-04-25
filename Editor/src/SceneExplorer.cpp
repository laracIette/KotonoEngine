#include "SceneExplorer.h"
#include "GameManager.h"
#include "SceneExplorerAddButton.h"
#include "SceneExplorerItem.h"
#include "SceneExplorerRemoveButton.h"
#include <kotono_core/Game.h>
#include <kotono_core/Scene.h>
#include <kotono_interface/widgets.h>

WidgetPtr WSceneExplorer::Build()
{
	UPtr addButtonBox{ Create<WBox>{}() };
	addButtonBox->SetChild(Create<WSceneExplorerAddButton>{}());
	addButtonBox->SetSize({ 80.0f, 40.0f });

	UPtr removeButtonBox{ Create<WBox>{}() };
	removeButtonBox->SetChild(Create<WSceneExplorerRemoveButton>{}());
	removeButtonBox->SetSize({ 80.0f, 40.0f });

	UPtr buttonRow{ Create<WRow>{}() };
	buttonRow->SetChildren({ addButtonBox, Create<WSpacer>{}(EFlex::Horizontal), removeButtonBox });


	UPtr headerText{ Create<WText>{}() };
	headerText->SetText("Scene Explorer");
	headerText->SetFontSize({ 20.0f, 24.0f });
	headerText->SetSpacing(-5.0f);


	UPtr itemListBg{ Create<WColor>{}() };
	itemListBg->SetColor(Colors::Black.WithAlpha(0.2f));

	UPtr itemList{ Create<WList>{}() };
	WidgetPool items{};
	if (UPtr scene{ Game.GetOpenedScene() })
	{
		for (auto& sceneObject : scene->SceneObjects())
		{
			items.Add(Create<WSceneExplorerItem>{}(sceneObject));
		}
	}
	itemList->SetChildren(items);

	UPtr itemListPadding{ Create<WPadding>{}() };
	itemListPadding->SetChild(itemList);
	itemListPadding->SetPadding(UPadding::All(5.0f));

	UPtr itemListStack{ Create<WStack>{}() };
	itemListStack->SetChildren({ itemListBg, itemListPadding });

	UPtr itemListStackPadding{ Create<WPadding>{}() };
	itemListStackPadding->SetChild(itemListStack);
	itemListPadding->SetPadding(UPadding::All(5.0f));


	UPtr column{ Create<WColumn>{}() };
	column->SetChildren({ buttonRow, headerText, itemListStackPadding });
	column->SetSpacing(10.0f);

	UPtr columnPadding{ Create<WPadding>{}() };
	columnPadding->SetChild(column);
	columnPadding->SetPadding(UPadding::All(10.0f));


	UPtr mainStackBg{ Create<WColor>{}() };
	mainStackBg->SetColor(Colors::White.WithValue(0.5f).WithAlpha(0.8f));


	UPtr mainStack{ Create<WStack>{}() };
	mainStack->SetChildren({ mainStackBg, columnPadding });

	return mainStack;
}

void WSceneExplorer::Display(UWidgetDisplaySettings displaySettings)
{
	Base::Display(displaySettings);

	GameManager.EventStateChanged().AddListener(this, &Self::OnGameStateChanged);
	
	if (UPtr scene{ Game.GetOpenedScene() })
	{
		scene->EventSceneObjectsUpdated().AddListener(this, &Self::Refresh);
	}
}

void WSceneExplorer::Remove()
{
	Base::Remove();

	GameManager.EventStateChanged().RemoveListener(this, &Self::OnGameStateChanged);
	
	if (UPtr scene{ Game.GetOpenedScene() })
	{
		scene->EventSceneObjectsUpdated().RemoveListener(this, &Self::Refresh);
	}
}

void WSceneExplorer::OnGameStateChanged(const EGameState gameState)
{
	Refresh();
}

#include "generated/SceneExplorer.generated.inl"
