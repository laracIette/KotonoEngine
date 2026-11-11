#include "ObjectPropertiesWindow.h"

#include "ObjectProperties/ObjectProperties.h"
#include <Scene/Scene.h>
#include <SceneObject/SceneObject.h>
#include <core_widgets.h>

WidgetPtr WObjectPropertiesWindow::Build()
{
    return (
        UCreate<WStack>{}()
        | (
            UCreate<WColor>{}()
            | Apply(&WColor::SetColor, Colors::Black.WithValue(0.05f))
        )
        | (
            UCreate<WPadding>{}()
            | Apply(&WPadding::SetPadding, UPadding::All(8.0f))
            | (
                UCreate<WList>{}()
                | Apply(&WList::SetSpacing, 10.0f)
                | (
                    UCreate<WWrap>{}()
                    | (
                        UCreate<WStack>{}()
                        | (
                            UCreate<WColor>{}()
                            | Apply(&WColor::SetColor, Colors::Black.WithValue(0.05f))
                        )
                        | (
                            UCreate<WText>{}()
                            | Apply(&WText::SetText, "Properties")
                        )
                    )
                )
                | (
                    objectPropertiesSocket_ = UCreate<WSocket>{}()
                    | (
                        objectProperties_ = UCreate<WObjectProperties>{}(GetScene()->GetSelectedObjects())
                    )
                )
            )
        )
    );
}

void WObjectPropertiesWindow::OnDisplayed()
{
    Base::OnDisplayed();

    GetScene()->GetEventSelectedObjectsUpdated().AddListener(this, &Self::OnSelectedObjectsUpdated);
}

void WObjectPropertiesWindow::OnRemoved()
{
    Base::OnRemoved();

    GetScene()->GetEventSelectedObjectsUpdated().RemoveListener(this, &Self::OnSelectedObjectsUpdated);
}

void WObjectPropertiesWindow::OnSelectedObjectsUpdated(std::span<UObjectPtr<TSceneObject> const> sceneObjects)
{
    Check(Throw, objectPropertiesSocket_, "objectPropertiesSocket_ is null");

    UObjectPtr const newObjectProperties{ UCreate<WObjectProperties>{}(sceneObjects) };
    objectPropertiesSocket_->SetChild(newObjectProperties);

    Delete(objectProperties_);
    objectProperties_ = newObjectProperties;
}

#include "ObjectPropertiesWindow.generated.inl"
