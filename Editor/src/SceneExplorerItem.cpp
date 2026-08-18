#include "SceneExplorerItem.h"
#include <kotono_interface/widgets.h>
#include <kotono_object/ObjectManager.h>
#include <kotono_object/SceneObject.h>

WSceneExplorerItem::WSceneExplorerItem(UPtr<TSceneObject> const& sceneObject)
	: sceneObject_{ sceneObject }
{
}

WidgetPtr WSceneExplorerItem::Build()
{
	bg_ = UCreate<WColor>{}();
	bg_->SetColor(Colors::Transparent);

	UPtr button{ UCreate<WButton>{}() };
	button->SetOnPressed([this]() {
		ObjectManager.SetSelectedObject(sceneObject_);
	});

	UPtr text{ UCreate<WText>{}() };
	text->SetText(sceneObject_ ? sceneObject_->GetName() : "");
	text->SetFontSize({ 20.0f, 24.0f });
	text->SetSpacing(-6.0f);


	const UChildOwnerTree widgetTree{ UCreate<WWrap>{}(),
		new UChildrenOwnerTree{ UCreate<WStack>{}(), {
			new UWidgetTreeLeaf{ bg_ },
			new UWidgetTreeLeaf{ button },
			new UWidgetTreeLeaf{ text },
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
	if (bg_)
	{
		bg_->SetColor(object == sceneObject_
			? Colors::Black.WithAlpha(0.2f)
			: Colors::Transparent
		);
	}
}

#include "generated/SceneExplorerItem.generated.inl"
