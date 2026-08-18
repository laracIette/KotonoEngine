#include "SceneExplorer.h"

#include "SceneExplorerAddButton.h"
#include "SceneExplorerItem.h"
#include "SceneExplorerRemoveButton.h"
#include <kotono_interface/widgets.h>
#include <kotono_object/Scene.h>

WidgetPtr WSceneExplorer::Build()
{
	UPtr headerText{ UCreate<WText>{ "Scene Explorer Text" }() };
	headerText->SetText("Scene Explorer");
	headerText->SetFontSize({ 20.0f, 24.0f });
	headerText->SetSpacing(-5.0f);

	itemList_ = UCreate<WList>{ "Scene Explorer Item List" }();
	PopulateItemList();

	const UChildrenOwnerTree widgetTree(UCreate<WStack>{}(), {
		new UWidgetTreeLeaf(UCreate<WColor>{}(Colors::White.WithValue(0.5f).WithAlpha(0.4f))),
		new UChildOwnerTree(UCreate<WPadding>{}(UPadding::All(10.0f)),
			new UChildrenOwnerTree(UCreate<WColumn>{}(10.0f), {
				new UChildrenOwnerTree(UCreate<WRow>{}(), {
					new UChildOwnerTree(UCreate<WBox>{ "Scene Explorer Add Button Box" }(glm::vec2{ 80.0f, 40.0f }),
						new UWidgetTreeLeaf(UCreate<WSceneExplorerAddButton>{}(GetSceneContext()))
					),
					new UWidgetTreeLeaf(UCreate<WSpacer>{}(EAxis::Horizontal)),
					new UChildOwnerTree(UCreate<WBox>{ "Scene Explorer Remove Button Box" }(glm::vec2{ 80.0f, 40.0f }),
						new UWidgetTreeLeaf(UCreate<WSceneExplorerRemoveButton>{}(GetSceneContext()))
					),
				}),
				new UWidgetTreeLeaf(headerText),
				new UChildOwnerTree(UCreate<WPadding>{}(UPadding::All(5.0f)),
					new UChildrenOwnerTree(UCreate<WStack>{}(), {
						new UWidgetTreeLeaf(UCreate<WColor>{}(Colors::Black.WithAlpha(0.5f))),
						new UChildOwnerTree(UCreate<WPadding>{}(UPadding::All(5.0f)),
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

void WSceneExplorer::Display(UWidgetDisplaySettings const& displaySettings)
{
	Base::Display(displaySettings);

	GetScene()->GetEventGameStateUpdated().AddListener(this, &Self::OnGameStateChanged);
	GetScene()->GetEventSceneObjectsUpdated().AddListener(this, &Self::PopulateItemList);
}

void WSceneExplorer::Remove()
{
	Base::Remove();

	GetScene()->GetEventGameStateUpdated().RemoveListener(this, &Self::OnGameStateChanged);
	GetScene()->GetEventSceneObjectsUpdated().RemoveListener(this, &Self::PopulateItemList);
}

void WSceneExplorer::OnGameStateChanged(EGameState gameState) const
{
	PopulateItemList();
}

void WSceneExplorer::PopulateItemList() const
{
	if (itemList_)
	{
		UAutoDelete<WWidget> const itemListChildren{ itemList_->GetChildren() };

		WidgetSet items{};
		for (auto const& sceneObject : GetScene()->GetSceneObjects())
		{
			items.Add(UCreate<WSceneExplorerItem>{}(sceneObject));
		}
		itemList_->SetChildren(items);
	}
}

#include "generated/SceneExplorer.generated.inl"
