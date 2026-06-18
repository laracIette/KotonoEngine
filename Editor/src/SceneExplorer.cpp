#include "SceneExplorer.h"
#include "SceneExplorerAddButton.h"
#include "SceneExplorerItem.h"
#include "SceneExplorerRemoveButton.h"
#include <kotono_core/Game.h>
#include <kotono_core/Scene.h>
#include <kotono_interface/widgets.h>

WidgetPtr WSceneExplorer::Build()
{
	UPtr addButtonBox{ UCreate<WBox>{ "Scene Explorer Add Button Box" }() };
	addButtonBox->SetSize({ 80.0f, 40.0f });

	UPtr removeButtonBox{ UCreate<WBox>{ "Scene Explorer Remove Button Box" }() };
	removeButtonBox->SetSize({ 80.0f, 40.0f });


	UPtr headerText{ UCreate<WText>{ "Scene Explorer Text" }() };
	headerText->SetText("Scene Explorer");
	headerText->SetFontSize({ 20.0f, 24.0f });
	headerText->SetSpacing(-5.0f);


	UPtr itemListBg{ UCreate<WColor>{}() };
	itemListBg->SetColor(Colors::Black.WithAlpha(0.2f));

	itemList_ = UCreate<WList>{ "Scene Explorer Item List" }();
	PopulateItemList();

	UPtr itemListPadding{ UCreate<WPadding>{}() };
	itemListPadding->SetPadding(UPadding::All(5.0f));

	UPtr itemListStackPadding{ UCreate<WPadding>{}() };
	itemListPadding->SetPadding(UPadding::All(5.0f));


	UPtr column{ UCreate<WColumn>{}() };
	column->SetSpacing(10.0f);

	UPtr columnPadding{ UCreate<WPadding>{}() };
	columnPadding->SetPadding(UPadding::All(10.0f));


	UPtr mainStackBg{ UCreate<WColor>{}() };
	mainStackBg->SetColor(Colors::White.WithValue(0.5f).WithAlpha(0.8f));

	const UChildrenOwnerTree widgetTree(UCreate<WStack>{}(), {
		new UWidgetTreeLeaf(mainStackBg),
		new UChildOwnerTree(columnPadding, 
			new UChildrenOwnerTree(column, {
				new UChildrenOwnerTree(UCreate<WRow>{}(), {
					new UChildOwnerTree(addButtonBox, 
						new UWidgetTreeLeaf(UCreate<WSceneExplorerAddButton>{}())
					),
					new UWidgetTreeLeaf(UCreate<WSpacer>{}(EAxis::Horizontal)),
					new UChildOwnerTree(removeButtonBox, 
						new UWidgetTreeLeaf(UCreate<WSceneExplorerRemoveButton>{}())
					),
				}),
				new UWidgetTreeLeaf(headerText),
				new UChildOwnerTree(itemListStackPadding, 
					new UChildrenOwnerTree(UCreate<WStack>{}(), {
						new UWidgetTreeLeaf(itemListBg),
						new UChildOwnerTree(itemListPadding, 
							new UWidgetTreeLeaf(itemList_)
						),
					})
				),
			})
		),
	});
	widgetTree.Link();

	return widgetTree.Widget();
}

void WSceneExplorer::Display(UWidgetDisplaySettings displaySettings)
{
	Base::Display(displaySettings);

	Game.EventStateChanged().AddListener(this, &Self::OnGameStateChanged);
	
	if (UPtr scene{ Game.GetOpenedScene() })
	{
		scene->EventSceneObjectsUpdated().AddListener(this, &Self::PopulateItemList);
	}
}

void WSceneExplorer::Remove()
{
	Base::Remove();

	Game.EventStateChanged().RemoveListener(this, &Self::OnGameStateChanged);
	
	if (UPtr scene{ Game.GetOpenedScene() })
	{
		scene->EventSceneObjectsUpdated().RemoveListener(this, &Self::PopulateItemList);
	}
}

void WSceneExplorer::OnGameStateChanged(const EGameState gameState)
{
	PopulateItemList();
}

void WSceneExplorer::PopulateItemList() const
{
	if (itemList_)
	{
		const UAutoDelete itemListChildren{ itemList_->GetChildren() };

		WidgetPool items{};
		if (UPtr scene{ Game.GetOpenedScene() })
		{
			for (auto& sceneObject : scene->SceneObjects())
			{
				items.Add(UCreate<WSceneExplorerItem>{}(sceneObject));
			}
		}
		itemList_->SetChildren(items);
	}
}

#include "generated/SceneExplorer.generated.inl"
