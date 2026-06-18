#include "GameStateButton.h"
#include <kotono_core/Game.h>
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
    playPauseBg_->SetColor(Game.IsPlaying()
        ? Colors::White.WithValue(0.5f)
        : Colors::Green
    );

    UPtr playPauseButton{ UCreate<WButton>{}() };
    playPauseButton->SetOnPressed([]() { SwitchPlayPause(); });


    UPtr stopBox{ UCreate<WBox>{}() };
    stopBox->SetSize({ 64.0f, 64.0f });

    stopBg_ = UCreate<WColor>{}();
    stopBg_->SetColor(Game.IsStopped()
        ? Colors::Red.WithAlpha(0.1f)
        : Colors::Red
    );

    UPtr stopButton{ UCreate<WButton>{}() };
    stopButton->SetOnPressed([]() {
        if (!Game.IsStopped())
        {
            Game.Stop();
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
    Game.EventStateChanged().AddListener(this, &Self::OnGameStateChanged);
}

void WGameStateButton::Remove()
{
    Base::Remove();

    Keyboard.EventKey(EKey::Space, EInputState::Pressed).RemoveListener(this, &WGameStateButton::OnKeyboardSpaceKeyPressed);
    Game.EventStateChanged().RemoveListener(this, &Self::OnGameStateChanged);
}

void WGameStateButton::OnKeyboardSpaceKeyPressed() const
{
    SwitchPlayPause();
}

void WGameStateButton::OnGameStateChanged(const EGameState gameState) const
{
    playPauseBg_->SetColor(Game.IsPlaying()
        ? Colors::White.WithValue(0.5f)
        : Colors::Green
    );
    stopBg_->SetColor(Game.IsStopped()
        ? Colors::Red.WithAlpha(0.1f)
        : Colors::Red
    );
}

void SwitchPlayPause()
{
    if (Game.IsPlaying())
    {
        Game.Stop();
    }
    else
    {
        Game.Play();
    }
}

#include "generated/GameStateButton.generated.inl"
