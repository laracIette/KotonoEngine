#include "GameStateButton.h"
#include "GameManager.h"
#include <kotono_interface/widgets.h>

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
                                    if (GameManager.IsPlaying())
                                    {
                                        GameManager.Pause();
                                    }
                                    else
                                    {
                                        GameManager.Play();
                                    }
                                });
                            },
                        }),
                        GameManager.IsPlaying() 
                            ? new WColor({ UColor::White().WithValue(0.5f) })
                            : new WColor({ UColor::Green() })
                    },
                }),
            }),
            new WBox({
                .size = { 64.0f, 64.0f },
                .child = new WStack({
                    .children = {
                        new WButton({
                            .onPress = [this]() {
                                if (GameManager.IsStopped())
                                {
                                    return;
                                }
                                SetState([]() {
                                    GameManager.Stop();
                                });
                            },
                        }),
                        GameManager.IsStopped()
                            ? new WColor({ UColor::Red().WithAlpha(0.1f) })
                            : new WColor({ UColor::Red() })
                    },
                }),
            }),
        },
    });
}
