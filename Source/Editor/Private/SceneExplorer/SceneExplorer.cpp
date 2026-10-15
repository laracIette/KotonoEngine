#include "SceneExplorer.h"

#include "SceneExplorerItem/SceneExplorerItem.h"
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
						itemList_ = UCreate<WList>{ "Scene Explorer Item List" }()
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

auto WSceneExplorer::MakeItems(std::span<UObjectPtr<TSceneObject> const> sceneObjects) const -> WidgetSet
{
	return sceneObjects
		| std::views::transform([this](UObjectPtr<TSceneObject> const& sceneObject) {
			return UCreate<WSceneExplorerItem>{}(GetScene(), sceneObject);
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
