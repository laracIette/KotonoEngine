#include "PropertiesWindow.h"
#include "ObjectProperties.h"
#include <kotono_common/log.h>
#include <kotono_core/InterfaceComponent.h>
#include <kotono_core/InterfaceObject.h>
#include <kotono_core/SceneComponent.h>
#include <kotono_core/SceneObject.h>
#include <kotono_input/Mouse.h>
#include <kotono_interface/widgets.h>
#include <kotono_object/ObjectManager.h>
#include <kotono_platform/glm_utils.h>

WidgetPtr WPropertiesWindow::Build()
{
    UPtr propertiesText{ UCreate<WText>{}() };
    propertiesText->SetText("Properties");
    propertiesText->SetSpacing(-20.0f);

    UPtr propertiesTextBg{ UCreate<WColor>{}() };
    propertiesTextBg->SetColor(Colors::Black.WithAlpha(0.5f));

    UPtr propertiesTextStack{ UCreate<WStack>{}() };
    propertiesTextStack->SetChildren({ propertiesTextBg, propertiesText });

    UPtr propertiesTextWrap{ UCreate<WWrap>{}() };
    propertiesTextWrap->SetChild(propertiesTextStack);


    objectProperties_ = UCreate<WObjectProperties>{}(ObjectManager.GetSelectedObject());

    mainList_ = UCreate<WList>{}();
    mainList_->SetChildren({ propertiesTextWrap, objectProperties_ });
    mainList_->SetSpacing(10.0f);

    UPtr mainListPadding{ UCreate<WPadding>{}() };
    mainListPadding->SetPadding(UPadding::All(8.0f));

    UPtr mainListBg{ UCreate<WColor>{}() };
    mainListBg->SetColor(Colors::Blue.WithAlpha(0.5f));

    const auto widgetTree{ UChildOwnerTree{ UCreate<WWrap>{}(),
        new UChildrenOwnerTree{ UCreate<WStack>{}(), {
            new UWidgetTreeLeaf{ mainListBg },
            new UChildOwnerTree{ mainListPadding, 
                new UWidgetTreeLeaf{ mainList_ }
            },
        } }
    } };
    widgetTree.Link();

    return widgetTree.Widget();

    //const auto widgetTree{ UChildOwnerTree{ UCreate<WWrap>{}(),
    //    new UChildrenOwnerTree{ UCreate<WStack>{}(), {
    //        new UWidgetTreeLeaf{ mainListBg },
    //        new UChildOwnerTree{ mainListPadding,
    //            new UChildrenOwnerTree{ mainList, {
    //                new UChildOwnerTree{ UCreate<WWrap>{}(),
    //                    new UChildrenOwnerTree{ UCreate<WStack>{}(), {
    //                        new UWidgetTreeLeaf{ propertiesTextBg },
    //                        new UWidgetTreeLeaf{ propertiesText },
    //                    } }
    //                },
    //                new UWidgetTreeLeaf{ UCreate<WObjectProperties>{}(selectedObject) },
    //            } }
    //        },
    //    } }
    //} };
}

void WPropertiesWindow::Display(UWidgetDisplaySettings displaySettings)
{
    Base::Display(displaySettings);

    ObjectManager.EventSelectedObjectChanged().AddListener(this, &Self::OnSelectedObjectChanged);
}

void WPropertiesWindow::Remove()
{
    Base::Remove();

    ObjectManager.EventSelectedObjectChanged().RemoveListener(this, &Self::OnSelectedObjectChanged);
}

void WPropertiesWindow::OnSelectedObjectChanged(const UPtr<KObject> object)
{
    if (mainList_)
    {
        auto children{ mainList_->GetChildren() };
        if (objectProperties_)
        {
            children.Remove(objectProperties_);
        }

        UPtr newObjectProperties{ UCreate<WObjectProperties>{}(object) };
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
