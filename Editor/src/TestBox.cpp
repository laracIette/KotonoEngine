#include "TestBox.h"
#include <kotono_core/ObjectManager.h>
#include <kotono_core/Object.h>
#include <kotono_interface/widgets.h>
#include <kotono_common/log.h>

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
