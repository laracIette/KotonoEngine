#include "PropertiesWindow.h"

#include "ObjectProperties/ObjectProperties.h"
#include <Scene/Scene.h>
#include <SceneObject/SceneObject.h>
#include <core_widgets.h>

WidgetPtr WPropertiesWindow::Build()
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
                mainList_ = UCreate<WList>{}()
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
                    objectProperties_ = UCreate<WObjectProperties>{}(GetScene()->GetSelectedObject())
                )
            )
        )
    );
}

void WPropertiesWindow::OnDisplayed()
{
    Base::OnDisplayed();

    GetScene()->GetEventSelectedObjectChanged().AddListener(this, &Self::OnSelectedObjectChanged);
}

void WPropertiesWindow::OnRemoved()
{
    Base::OnRemoved();

    GetScene()->GetEventSelectedObjectChanged().RemoveListener(this, &Self::OnSelectedObjectChanged);
}

void WPropertiesWindow::OnSelectedObjectChanged(UObjectPtr<TSceneObject> const& sceneObject)
{
    if (mainList_)
    {
        UObjectPtr const newObjectProperties{ UCreate<WObjectProperties>{}(sceneObject) };

        mainList_->ReplaceChild(objectProperties_, newObjectProperties);
    	
    	Delete(objectProperties_);

    	objectProperties_ = newObjectProperties;
    }
}

#include "PropertiesWindow.generated.inl"
