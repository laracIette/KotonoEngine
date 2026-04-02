#include "GameStateButton.h"
#include "GameManager.h"
#include <kotono_interface/widgets.h>
#include <kotono_input/Keyboard.h>

WidgetPtr WGameStateButton::Build()
{
    Keyboard.EventKey(EKey::Space, EInputState::Pressed).AddListener(this, &WGameStateButton::OnKeyboardSpaceKeyPressed);

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

void WGameStateButton::Cleanup()
{
    Keyboard.EventKey(EKey::Space, EInputState::Pressed).RemoveListener(this, &WGameStateButton::OnKeyboardSpaceKeyPressed);

    WWidget::Cleanup();
}

void WGameStateButton::OnKeyboardSpaceKeyPressed()
{
    SetState([]()
    {
        if (GameManager.IsPlaying())
        {
            GameManager.Stop();
        }
        else
        {
            GameManager.Play();
        }
    });
}
