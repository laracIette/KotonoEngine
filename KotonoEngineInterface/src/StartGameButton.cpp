#include "StartGameButton.h"
#include <kotono_interface/widgets.h>
#include <kotono_engine/Engine.h>
#include <kotono_engine/Game.h>

WWidget* WStartGameButton::Build()
{
    return new WBox({
        .size = { 64.0f, 64.0f },
        .child = new WStack({
            .children = {
                new WButton({
                    .onPress = [this]() {
                        SetState([]() {
                            if (Engine.Game().IsPlaying())
                            {
                                Engine.Game().Stop();
                            }
                            else
                            {
                                Engine.Game().Play();
                            }
                        });
                    },
                }),
                Engine.Game().IsPlaying() ? 
                    new WColor({ KtColor::Red() }) : 
                    new WColor({ KtColor::Green() })
            },
        }),
    });
}
