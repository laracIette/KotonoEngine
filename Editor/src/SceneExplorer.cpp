#include "SceneExplorer.h"

#include "SceneExplorerAddButton.h"
#include "SceneExplorerItem.h"
#include "SceneExplorerRemoveButton.h"
#include <kotono_core/Scene.h>
#include <kotono_interface/widgets.h>

WidgetPtr WSceneExplorer::Build()
{
	itemList_ = UCreate<WList>{ "Scene Explorer Item List" }();
	PopulateItemList(GetScene()->GetSceneObjects());

	const UChildrenOwnerTree widgetTree(UCreate<WStack>{}(), {
		new UWidgetTreeLeaf(UCreate<WColor>{}(Colors::White.WithValue(0.5f).WithAlpha(0.4f))),
		new UChildOwnerTree(UCreate<WPadding>{}(UPadding::All(10.0f)),
			new UChildrenOwnerTree(UCreate<WColumn>{}(10.0f), {
				new UChildrenOwnerTree(UCreate<WRow>{}(), {
					new UChildOwnerTree(UCreate<WBox>{ "Scene Explorer Add Button Box" }(glm::vec2{ 80.0f, 40.0f }),
						new UWidgetTreeLeaf(UCreate<WSceneExplorerAddButton>{}(GetScene()))
					),
					new UWidgetTreeLeaf(UCreate<WSpacer>{}(EAxis::Horizontal)),
					new UChildOwnerTree(UCreate<WBox>{ "Scene Explorer Remove Button Box" }(glm::vec2{ 80.0f, 40.0f }),
						new UWidgetTreeLeaf(UCreate<WSceneExplorerRemoveButton>{}(GetScene()))
					),
				}),
				new UWidgetTreeLeaf(UCreate<WText>{ "Scene Explorer Text" }("Scene Explorer")),
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

	GetScene()->GetEventGameStateChanged().AddListener(this, &Self::OnGameStateChanged);
	GetScene()->GetEventSceneObjectsUpdated().AddListener(this, &Self::PopulateItemList);
}

void WSceneExplorer::Remove()
{
	Base::Remove();

	GetScene()->GetEventGameStateChanged().RemoveListener(this, &Self::OnGameStateChanged);
	GetScene()->GetEventSceneObjectsUpdated().RemoveListener(this, &Self::PopulateItemList);
}

void WSceneExplorer::OnGameStateChanged(EGameState gameState) const
{
	if (gameState != EGameState::Paused)
	{
		PopulateItemList(GetScene()->GetSceneObjects());
	}
}

void WSceneExplorer::PopulateItemList(std::span<UPtr<TSceneObject> const> sceneObjects) const
{
	if (itemList_)
	{
		UAutoDelete<WWidget> const itemListChildren{ itemList_->GetChildren() };

		WidgetSet items{};
		for (auto const& sceneObject : sceneObjects)
		{
			items.Add(UCreate<WSceneExplorerItem>{}(sceneObject));
		}
		itemList_->SetChildren(items);
	}
}

#include "generated/SceneExplorer.generated.inl"
