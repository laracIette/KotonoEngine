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
	UPtr button{ UCreate<WButton>{}() };
	button->SetOnActive([this]() {
		GetScene()->SelectObject(sceneObject_);
	});

	const UChildOwnerTree widgetTree{ UCreate<WWrap>{}(),
		new UChildrenOwnerTree{ UCreate<WStack>{}(), {
			new UWidgetTreeLeaf{ button },
			new UWidgetTreeLeaf{ UCreate<WText>{}(sceneObject_ ? sceneObject_->GetName() : "") },
		} }
	};
	widgetTree.Link();

	return widgetTree.Widget();
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
