#include "GameStateButton.h"
#include "GameManager.h"
#include <kotono_interface/widgets.h>
#include <kotono_input/Keyboard.h>

WidgetPtr WGameStateButton::Build()
{
    UPtr mainRow{ Create<WRow>{}() };
    mainRow->SetSpacing(5.0f);

    UPtr playPauseBox{ Create<WBox>{}() };
    playPauseBox->SetSize({ 64.0f, 64.0f });

    UPtr playPauseBg{ Create<WColor>{}() };
    playPauseBg->SetColor(GameManager.IsPlaying()
        ? Colors::White.WithValue(0.5f)
        : Colors::Green
    );

    UPtr playPauseButton{ Create<WButton>{}() };
    playPauseButton->SetOnPressed([this]() {
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
    });


    UPtr stopBox{ Create<WBox>{}() };
    stopBox->SetSize({ 64.0f, 64.0f });

    UPtr stopBg{ Create<WColor>{}() };
    stopBg->SetColor(GameManager.IsStopped()
        ? Colors::Red.WithAlpha(0.1f)
        : Colors::Red
    );

    UPtr stopButton{ Create<WButton>{}() };
    stopButton->SetOnPressed([this]() {
        if (GameManager.IsStopped())
        {
            return;
        }
        SetState([]() {
            GameManager.Stop();
        });
    });

    UChildrenOwnerTree(mainRow, {
        new UChildOwnerTree(playPauseBox,
            new UChildrenOwnerTree(Create<WStack>{}(), {
                new UWidgetTreeLeaf(playPauseBg),
                new UWidgetTreeLeaf(playPauseButton),
            })
        ),
        new UChildOwnerTree(stopBox,
            new UChildrenOwnerTree(Create<WStack>{}(), {
                new UWidgetTreeLeaf(stopBg),
                new UWidgetTreeLeaf(stopButton),
            })
        )
    }).Link();

    return mainRow;
}

void WGameStateButton::Display(UWidgetDisplaySettings displaySettings)
{
    Base::Display(displaySettings);

    Keyboard.EventKey(EKey::Space, EInputState::Pressed).AddListener(this, &WGameStateButton::OnKeyboardSpaceKeyPressed);
}

void WGameStateButton::Remove()
{
    Base::Remove();

    Keyboard.EventKey(EKey::Space, EInputState::Pressed).RemoveListener(this, &WGameStateButton::OnKeyboardSpaceKeyPressed);
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

#include "generated/GameStateButton.generated.inl"
