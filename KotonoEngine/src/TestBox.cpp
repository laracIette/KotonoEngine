#include "TestBox.h"
#include "Engine.h"
#include "Object.h"
#include <kotono_interface/widgets.h>
#include "log.h"

WWidget* WTestBox::Build()
{
    if (Engine.GetObjectManager().selectedObject)
    {
        return new WBox({
            .size = { 100.0f, 100.0f },
            .child = new WStack({
                .children = {
                    new WColor({ KtColor::Red() }),
                    new WButton({
                        .onPress = [this]() { 
                            SetState([]() { 
                                Engine.GetObjectManager().selectedObject->Delete();
                                Engine.GetObjectManager().selectedObject = nullptr;
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
