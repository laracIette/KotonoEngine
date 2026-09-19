#include "SceneExplorerRemoveButton.h"

#include <kotono_core/Scene.h>
#include <kotono_core/SceneObject.h>
#include <kotono_interface/widgets.h>

WidgetPtr WSceneExplorerRemoveButton::Build()
{
    bg_ = UCreate<WColor>{}();
    bg_->SetColor(GetScene()->GetSelectedObject()
        ? Colors::Red
        : Colors::Red.WithValue(0.1f)
    );

    UPtr text{ UCreate<WText>{}() };
    text->SetFontSize({ 14.0f, 18.0f });
    text->SetText("Remove");

    button_ = UCreate<WButton>{}();
    button_->SetIsEnabled(false);
    button_->SetOnClicked([this]() {
        if (UPtr selectedObject{ GetScene()->GetSelectedObject() })
        {
            GetScene()->Remove(selectedObject);
            GetScene()->SelectObject(nullptr);
            selectedObject->Delete();
        }
    });

    const auto widgetTree{ UChildrenOwnerTree{ UCreate<WStack>{ "Remove Button Stack" }(), {
        new UWidgetTreeLeaf{ button_ },
        new UChildOwnerTree{ UCreate<WCenter>{}(),
            new UWidgetTreeLeaf{ text },
        },
        new UWidgetTreeLeaf{ bg_ },
    } } };
    widgetTree.Link();

    return widgetTree.Widget();
}

void WSceneExplorerRemoveButton::Display(UWidgetDisplaySettings const& displaySettings)
{
    Base::Display(displaySettings);

    GetScene()->GetEventSelectedObjectChanged().AddListener(this, &Self::OnSelectedObjectChanged);
}

void WSceneExplorerRemoveButton::Remove()
{
    Base::Remove();

    GetScene()->GetEventSelectedObjectChanged().RemoveListener(this, &Self::OnSelectedObjectChanged);
}

void WSceneExplorerRemoveButton::OnSelectedObjectChanged(UPtr<TSceneObject> const& sceneObject) const
{
    bg_->SetColor(sceneObject
        ? Colors::Red
        : Colors::Red.WithValue(0.1f)
    );
    button_->SetIsEnabled(sceneObject);
}

#include "generated/SceneExplorerRemoveButton.generated.inl"
