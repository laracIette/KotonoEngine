#include "GameStateButton.h"
#include <kotono_interface/widgets.h>
#include "Editor.h"
#include "GameManager.h"

WWidget* WGameStateButton::Build()
{
    return new WRow({
        .spacing = 5.0f,
        .children = {
            new WBox({
                .size = { 64.0f, 64.0f },
                .child = new WStack({
                    .children = {
                        new WButton({
                            .onPress = [this]() {
                                SetState([]() {
                                    if (Editor.GameManager().IsPlaying())
                                    {
                                        Editor.GameManager().Pause();
                                    }
                                    else
                                    {
                                        Editor.GameManager().Play();
                                    }
                                });
                            },
                        }),
                        Editor.GameManager().IsPlaying() 
                            ? new WColor({ KtColor::White().WithValue(0.5f) })
                            : new WColor({ KtColor::Green() })
                    },
                }),
            }),
            new WBox({
                .size = { 64.0f, 64.0f },
                .child = new WStack({
                    .children = {
                        new WButton({
                            .onPress = [this]() {
                                if (Editor.GameManager().IsStopped())
                                {
                                    return;
                                }
                                SetState([]() {
                                    Editor.GameManager().Stop();
                                });
                            },
                        }),
                        Editor.GameManager().IsStopped()
                            ? new WColor({ KtColor::Red().WithAlpha(0.1f) })
                            : new WColor({ KtColor::Red() })
                    },
                }),
            }),
        },
    });
}
