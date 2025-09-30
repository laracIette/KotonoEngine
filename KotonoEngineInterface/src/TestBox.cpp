#include "TestBox.h"
#include <kotono_engine/Engine.h>
#include <kotono_engine/ObjectManager.h>
#include <kotono_engine/Object.h>
#include <kotono_interface/widgets.h>
#include "log.h"

WWidget* WTestBox::Build()
{
    if (Engine.ObjectManager().selectedObject)
    {
        return new WBox({
            .size = { 100.0f, 100.0f },
            .child = new WStack({
                .children = {
                    new WColor({ KtColor::Red() }),
                    new WButton({
                        .onPress = [this]() { 
                            SetState([]() { 
                                Engine.ObjectManager().selectedObject->Delete();
                                Engine.ObjectManager().selectedObject = nullptr;
                            });
                        },
                    }),
                },
            }),
        });
    }
    return new WBox({
        .size = { 100.0f, 100.0f },
        .child = new WColor({ KtColor::Green() }),
    });
}
