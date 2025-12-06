#include "TestBox.h"
#include <kotono_engine/ObjectManager.h>
#include <kotono_engine/Object.h>
#include <kotono_interface/widgets.h>
#include "log.h"

WWidget* WTestBox::Build()
{
    return new WBox({
        .size = { 100.0f, 100.0f },
        .child = new WStack({
            .children = {
                new WColor({ KtColor::Red() }),
                new WButton({
                    .onDown = [this]() { 
                        if (ObjectManager.SelectedObject())
                        {
                            SetState([]() {
                                ObjectManager.SelectedObject()->Delete();
                            });
                        }
                    },
                }),
            },
        }),
    });
}
