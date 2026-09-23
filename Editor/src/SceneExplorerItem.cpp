#include "SceneExplorerItem.h"

#include <kotono_core/Scene.h>
#include <kotono_core/SceneObject.h>
#include <kotono_interface/widgets.h>

WSceneExplorerItem::WSceneExplorerItem(UScene* scene, UPtr<TSceneObject> const& sceneObject)
	: Base(scene)
	, sceneObject_{ sceneObject }
{
}

WidgetPtr WSceneExplorerItem::Build()
{
	return (
		UCreate<WWrap>{}()
		| (
			UCreate<WStack>{}()
			| (
				UCreate<WButton>{}()
				| Apply(&WButton::SetIsSelectable, true)
				| Apply(&WButton::SetOnClicked, [this]() { GetScene()->SelectObject(sceneObject_); })
			)
			| (
				UCreate<WText>{}()
				| Apply(&WText::SetText, sceneObject_ ? sceneObject_->GetName() : "")
			)
		)
	);
}

void WSceneExplorerItem::Display(UWidgetDisplaySettings const& displaySettings)
{
	Base::Display(displaySettings);

	GetScene()->GetEventSelectedObjectChanged().AddListener(this, &Self::OnSelectedObjectChanged);
}

void WSceneExplorerItem::Remove()
{
	Base::Remove();

	GetScene()->GetEventSelectedObjectChanged().RemoveListener(this, &Self::OnSelectedObjectChanged);
}

void WSceneExplorerItem::OnSelectedObjectChanged(UPtr<TSceneObject> const& sceneObject)
{
}

#include "generated/SceneExplorerItem.generated.inl"
