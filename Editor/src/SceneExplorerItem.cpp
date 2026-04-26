#include "SceneExplorerItem.h"
#include <kotono_core/SceneObject.h>
#include <kotono_interface/widgets.h>
#include <kotono_object/ObjectManager.h>

WSceneExplorerItem::WSceneExplorerItem(const UPtr<TSceneObject>& sceneObject)
	: sceneObject_(sceneObject)
{
}

WidgetPtr WSceneExplorerItem::Build()
{
	UPtr color{ UCreate<WColor>{}() };
	color->SetColor(sceneObject_ && (ObjectManager.GetSelectedObject() == sceneObject_)
		? Colors::Black.WithAlpha(0.2f)
		: Colors::Transparent
	);

	UPtr button{ UCreate<WButton>{}() };
	button->SetOnPressed([this]() {
		SetState([this]() {
			ObjectManager.SetSelectedObject(sceneObject_);
		});
	});

	UPtr text{ UCreate<WText>{}() };
	text->SetText(sceneObject_ ? sceneObject_->GetName() : "");
	text->SetFontSize({ 20.0f, 24.0f });
	text->SetSpacing(-6.0f);


	const UChildOwnerTree widgetTree{ UCreate<WWrap>{}(),
		new UChildrenOwnerTree{ UCreate<WStack>{}(), {
			new UWidgetTreeLeaf{ color },
			new UWidgetTreeLeaf{ button },
			new UWidgetTreeLeaf{ text },
		} }
	};
	widgetTree.Link();

	return widgetTree.Widget();
}

void WSceneExplorerItem::Display(UWidgetDisplaySettings displaySettings)
{
	Base::Display(displaySettings);

	ObjectManager.EventSelectedObjectChanged().AddListener(this, &WSceneExplorerItem::Refresh);
}

void WSceneExplorerItem::Remove()
{
	Base::Remove();

	ObjectManager.EventSelectedObjectChanged().RemoveListener(this, &WSceneExplorerItem::Refresh);
}

#include "generated/SceneExplorerItem.generated.inl"
