#include "SceneExplorer.h"

#include "SceneExplorerItem/SceneExplorerItem.h"
#include "SelectableList/SelectableList.h"
#include "SelectableList/SelectableListItem.h"
#include <Scene/Scene.h>
#include <core_widgets.h>

WidgetPtr WSceneExplorer::Build()
{
	return (
		UCreate<WStack>{}()
		| (
			UCreate<WColor>{}()
			| Apply(&WColor::SetColor, Colors::White.WithValue(0.5f).WithAlpha(0.4f))
		)
		| (
			UCreate<WPadding>{}()
			| Apply(&WPadding::SetPadding, UPadding::All(5.0f))
			| (
				UCreate<WStack>{}()
				| (
					UCreate<WColor>{}()
					| Apply(&WColor::SetColor, Colors::Black.WithAlpha(0.5f))
				)
				| (
					UCreate<WPadding>{}()
					| Apply(&WPadding::SetPadding, UPadding::All(5.0f))
					| (
						itemList_ = UCreate<WSelectableList>{ "Scene Explorer Item List" }()
						| MakeItems(GetScene()->GetSceneObjects())
					)
				)
			)
		)
	);
}

void WSceneExplorer::OnDisplayed()
{
	Base::OnDisplayed();

	GetScene()->GetEventSceneObjectsUpdated().AddListener(this, &Self::UpdateItemList);
}

void WSceneExplorer::OnRemoved()
{
	Base::OnRemoved();

	GetScene()->GetEventSceneObjectsUpdated().RemoveListener(this, &Self::UpdateItemList);
}

auto WSceneExplorer::MakeItems(std::span<UObjectPtr<TSceneObject> const> sceneObjects) const -> USet<UObjectPtr<WSelectableListItem>>
{
	return sceneObjects
		| std::views::transform([this](UObjectPtr<TSceneObject> const& sceneObject) {
			return (
				UCreate<WSelectableListItem>{}()
				| Apply(&WSelectableListItem::SetOnSelected, [this, sceneObject]() {
					GetScene()->SelectObject(sceneObject);
				})
				| (
					UCreate<WText>{}()
					| Apply(&WText::SetText, sceneObject ? sceneObject->GetName() : "")
				)
			);
		})
		| std::ranges::to<USet>();
}

void WSceneExplorer::UpdateItemList(std::span<UObjectPtr<TSceneObject> const> sceneObjects) const
{
	if (itemList_)
	{
		UAutoDelete<WWidget> const itemListChildren{ itemList_->GetChildren() };

		auto const items{ MakeItems(sceneObjects) };
		itemList_->SetChildren(items);
	}
}

#include "SceneExplorer.generated.inl"
