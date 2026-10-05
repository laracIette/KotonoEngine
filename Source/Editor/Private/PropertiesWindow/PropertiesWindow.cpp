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

void WPropertiesWindow::Display(UWidgetDisplaySettings const& displaySettings)
{
    Base::Display(displaySettings);

    GetScene()->GetEventSelectedObjectChanged().AddListener(this, &Self::OnSelectedObjectChanged);
}

void WPropertiesWindow::Remove()
{
    Base::Remove();

    GetScene()->GetEventSelectedObjectChanged().RemoveListener(this, &Self::OnSelectedObjectChanged);
}

void WPropertiesWindow::OnSelectedObjectChanged(UPtr<TSceneObject> const& sceneObject)
{
    if (mainList_)
    {
        UPtr const newObjectProperties{ UCreate<WObjectProperties>{}(sceneObject) };

        mainList_->ReplaceChild(objectProperties_, newObjectProperties);

        if (objectProperties_)
        {
            objectProperties_->Delete();
        }

        objectProperties_ = newObjectProperties;
    }
}

#include "PropertiesWindow.generated.inl"
