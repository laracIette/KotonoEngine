#include "SceneExplorerItem.h"

#include <kotono_core/ObjectManager.h>
#include <kotono_core/SceneObject.h>
#include <kotono_interface/widgets.h>

WSceneExplorerItem::WSceneExplorerItem(UPtr<TSceneObject> const& sceneObject)
	: sceneObject_{ sceneObject }
{
}

WidgetPtr WSceneExplorerItem::Build()
{
	UPtr button{ UCreate<WButton>{}() };
	button->SetOnActive([this]() {
		ObjectManager.SetSelectedObject(sceneObject_);
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

	ObjectManager.EventSelectedObjectChanged().AddListener(this, &Self::OnSelectedObjectChanged);
}

void WSceneExplorerItem::Remove()
{
	Base::Remove();

	ObjectManager.EventSelectedObjectChanged().RemoveListener(this, &Self::OnSelectedObjectChanged);
}

void WSceneExplorerItem::OnSelectedObjectChanged(UPtr<KObject> const& object)
{
}

#include "generated/SceneExplorerItem.generated.inl"
