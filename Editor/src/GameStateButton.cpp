#include "GameStateButton.h"
#include "GameManager.h"
#include <kotono_interface/widgets.h>

WidgetPtr WGameStateButton::Build()
{
    return new WRow({
        .spacing = 5.0f,
        .children = {
            new WBox({
                .size = { 64.0f, 64.0f },
                .child = new WStack({
                    .children = {
                        new WButton({
                            .onPressed = [this]() {
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
                            ? new WColor({ Colors::White.WithValue(0.5f) })
                            : new WColor({ Colors::Green })
                    },
                }),
            }),
            new WBox({
                .size = { 64.0f, 64.0f },
                .child = new WStack({
                    .children = {
                        new WButton({
                            .onPressed = [this]() {
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
                            ? new WColor({ Colors::Red.WithAlpha(0.1f) })
                            : new WColor({ Colors::Red })
                    },
                }),
            }),
        },
    });
}
