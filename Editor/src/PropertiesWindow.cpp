#include "PropertiesWindow.h"

#include "ObjectProperties.h"
#include <kotono_core/Scene.h>
#include <kotono_core/SceneObject.h>
#include <kotono_interface/widgets.h>

WidgetPtr WPropertiesWindow::Build()
{
    return (
        UCreate<WWrap>{}()
        | (
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
                                UCreate<WColor>{}(Colors::Black.WithValue(0.05f))
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
        auto children{ mainList_->GetChildren() };
        if (objectProperties_)
        {
            children.Remove(objectProperties_);
        }

        UPtr newObjectProperties{ UCreate<WObjectProperties>{}(sceneObject) };
        children.Add(newObjectProperties);

        mainList_->SetChildren(children);

        if (objectProperties_)
        {
            objectProperties_->Delete();
        }
        objectProperties_ = newObjectProperties;
    }
}

#include "generated/PropertiesWindow.generated.inl"
