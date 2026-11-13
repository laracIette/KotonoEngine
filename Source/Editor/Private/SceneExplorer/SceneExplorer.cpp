#include "SceneExplorer.h"

#include "SelectableList/SelectableList.h"
#include "SelectableList/SelectableListItem.h"
#include <Scene/Scene.h>
#include <core_widgets.h>

auto WSceneExplorer::Build() -> WidgetPtr
{
	return (
		UCreate<WPadding>{}()
		| Apply(&WPadding::SetPadding, UPadding::All(5.0f))
		| (
			itemList_ = UCreate<WSelectableList>{ "Scene Explorer Item List" }()
			| Apply(&WSelectableList::SetContainer, (
			    UCreate<WList>{}()
			    | Apply(&WList::SetSpacing, 2.0f)
			))
			| (
				MakeItems(GetScene()->GetSceneObjects())
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

auto WSceneExplorer::OnKeyboardKey(EKey key, EInputState inputState, EModifier modifier) -> b8
{
    if (inputState != EInputState::Pressed)
    {
        return INPUT_UNHANDLED;
    }

    if (key == EKey::Delete || key == EKey::Backspace)
    {
        Delete(GetScene()->GetSelectedObjects());
        GetScene()->DeselectAllObjects();
        return INPUT_HANDLED;
    }

    return INPUT_UNHANDLED;
}

auto WSceneExplorer::MakeItems(std::span<UObjectPtr<TSceneObject> const> sceneObjects) const -> USet<UObjectPtr<WSelectableListItem>>
{
	return sceneObjects
        | filter_valid
		| std::views::transform([this](UObjectPtr<TSceneObject> const& sceneObject) {
			return (
				UCreate<WSelectableListItem>{}()
				| Apply(&WSelectableListItem::SetWrapAxis, EAxis::Vertical)
				| Apply(&WSelectableListItem::SetIsSelected, [this, sceneObject]() {
					return GetScene()->IsObjectSelected(sceneObject);
				})
				| Apply(&WSelectableListItem::SetOnSelected, [this, sceneObject]() {
				    GetScene()->SelectObject(sceneObject);
				})
				| Apply(&WSelectableListItem::SetOnDeselected, [this, sceneObject]() {
				    GetScene()->DeselectObject(sceneObject);
				})
				| (
					UCreate<WText>{}()
					| Apply(&WText::SetText, sceneObject->GetName())
				)
			);
		})
		| std::ranges::to<USet>();
}

void WSceneExplorer::UpdateItemList(std::span<UObjectPtr<TSceneObject> const> sceneObjects) const
{
	if (itemList_)
	{
		UAutoDelete const itemListChildren{ itemList_->GetChildren() };

		itemList_->SetChildren(MakeItems(sceneObjects));
	}
}

#include "SceneExplorer.generated.inl"
