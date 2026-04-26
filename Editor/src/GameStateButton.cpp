#include "GameStateButton.h"
#include "GameManager.h"
#include <kotono_interface/widgets.h>
#include <kotono_input/Keyboard.h>

WidgetPtr WGameStateButton::Build()
{
    UPtr mainRow{ UCreate<WRow>{}() };
    mainRow->SetSpacing(5.0f);

    UPtr playPauseBox{ UCreate<WBox>{}() };
    playPauseBox->SetSize({ 64.0f, 64.0f });

    UPtr playPauseBg{ UCreate<WColor>{}() };
    playPauseBg->SetColor(GameManager.IsPlaying()
        ? Colors::White.WithValue(0.5f)
        : Colors::Green
    );

    UPtr playPauseButton{ UCreate<WButton>{}() };
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


    UPtr stopBox{ UCreate<WBox>{}() };
    stopBox->SetSize({ 64.0f, 64.0f });

    UPtr stopBg{ UCreate<WColor>{}() };
    stopBg->SetColor(GameManager.IsStopped()
        ? Colors::Red.WithAlpha(0.1f)
        : Colors::Red
    );

    UPtr stopButton{ UCreate<WButton>{}() };
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
            new UChildrenOwnerTree(UCreate<WStack>{}(), {
                new UWidgetTreeLeaf(playPauseBg),
                new UWidgetTreeLeaf(playPauseButton),
            })
        ),
        new UChildOwnerTree(stopBox,
            new UChildrenOwnerTree(UCreate<WStack>{}(), {
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
