#include "GameStateButton.h"
#include "GameManager.h"
#include <kotono_interface/widgets.h>
#include <kotono_input/Keyboard.h>

static void SwitchPlayPause();

WidgetPtr WGameStateButton::Build()
{
    UPtr mainRow{ UCreate<WRow>{}() };
    mainRow->SetSpacing(5.0f);

    UPtr playPauseBox{ UCreate<WBox>{}() };
    playPauseBox->SetSize({ 64.0f, 64.0f });

    playPauseBg_ = UCreate<WColor>{}();
    playPauseBg_->SetColor(GameManager.IsPlaying()
        ? Colors::White.WithValue(0.5f)
        : Colors::Green
    );

    UPtr playPauseButton{ UCreate<WButton>{}() };
    playPauseButton->SetOnPressed([]() { SwitchPlayPause(); });


    UPtr stopBox{ UCreate<WBox>{}() };
    stopBox->SetSize({ 64.0f, 64.0f });

    stopBg_ = UCreate<WColor>{}();
    stopBg_->SetColor(GameManager.IsStopped()
        ? Colors::Red.WithAlpha(0.1f)
        : Colors::Red
    );

    UPtr stopButton{ UCreate<WButton>{}() };
    stopButton->SetOnPressed([]() {
        if (!GameManager.IsStopped())
        {
            GameManager.Stop();
        }
    });

    UChildrenOwnerTree(mainRow, {
        new UChildOwnerTree(playPauseBox,
            new UChildrenOwnerTree(UCreate<WStack>{}(), {
                new UWidgetTreeLeaf(playPauseBg_),
                new UWidgetTreeLeaf(playPauseButton),
            })
        ),
        new UChildOwnerTree(stopBox,
            new UChildrenOwnerTree(UCreate<WStack>{}(), {
                new UWidgetTreeLeaf(stopBg_),
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
    GameManager.EventStateChanged().AddListener(this, &Self::OnGameStateChanged);
}

void WGameStateButton::Remove()
{
    Base::Remove();

    Keyboard.EventKey(EKey::Space, EInputState::Pressed).RemoveListener(this, &WGameStateButton::OnKeyboardSpaceKeyPressed);
    GameManager.EventStateChanged().RemoveListener(this, &Self::OnGameStateChanged);
}

void WGameStateButton::OnKeyboardSpaceKeyPressed() const
{
    SwitchPlayPause();
}

void WGameStateButton::OnGameStateChanged(const EGameState gameState) const
{
    playPauseBg_->SetColor(GameManager.IsPlaying()
        ? Colors::White.WithValue(0.5f)
        : Colors::Green
    );
    stopBg_->SetColor(GameManager.IsStopped()
        ? Colors::Red.WithAlpha(0.1f)
        : Colors::Red
    );
}

void SwitchPlayPause()
{
    if (GameManager.IsPlaying())
    {
        GameManager.Stop();
    }
    else
    {
        GameManager.Play();
    }
}

#include "generated/GameStateButton.generated.inl"
