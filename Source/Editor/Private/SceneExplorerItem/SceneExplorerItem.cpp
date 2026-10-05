#include "SceneExplorerItem.h"

#include <Scene/Scene.h>
#include <SceneObject/SceneObject.h>
#include <core_widgets.h>

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
				| Apply(&WButton::SetOnClicked, [this]() { GetScene()->SelectObject(sceneObject_); })
			)
			| (
				UCreate<WText>{}()
				| Apply(&WText::SetText, sceneObject_ ? sceneObject_->GetName() : "")
			)
		)
	);
}

#include "generated/SceneExplorerItem.generated.inl"
