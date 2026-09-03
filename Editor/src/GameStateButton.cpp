#include "GameStateButton.h"

#include <kotono_input/Keyboard.h>
#include <kotono_interface/widgets.h>
#include <kotono_core/Scene.h>

static void SwitchPlayPause();

WidgetPtr WGameStateButton::Build()
{
    UPtr mainRow{ UCreate<WRow>{}() };
    mainRow->SetSpacing(5.0f);

    UPtr playPauseBox{ UCreate<WBox>{}() };
    playPauseBox->SetSize({ 64.0f, 64.0f });

    playPauseBg_ = UCreate<WColor>{}();
    playPauseBg_->SetColor(GetScene()->GetIsGamePlaying()
        ? Colors::White.WithValue(0.5f)
        : Colors::Green
    );

    UPtr playPauseButton{ UCreate<WButton>{}() };
    playPauseButton->SetOnPressed([this]() { SwitchPlayPause(); });


    UPtr stopBox{ UCreate<WBox>{}() };
    stopBox->SetSize({ 64.0f, 64.0f });

    stopBg_ = UCreate<WColor>{}();
    stopBg_->SetColor(GetScene()->GetIsGameStopped()
        ? Colors::Red.WithAlpha(0.1f)
        : Colors::Red
    );

    UPtr stopButton{ UCreate<WButton>{}() };
    stopButton->SetOnPressed([this]() {
        if (!GetScene()->GetIsGameStopped())
        {
            GetScene()->StopGame();
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

void WGameStateButton::Display(UWidgetDisplaySettings const& displaySettings)
{
    Base::Display(displaySettings);

    Keyboard.GetEventKey(EKey::Space, EInputState::Pressed).AddListener(this, &WGameStateButton::OnKeyboardSpaceKeyPressed);
    GetScene()->GetEventGameStateUpdated().AddListener(this, &Self::OnGameStateChanged);
}

void WGameStateButton::Remove()
{
    Base::Remove();

    Keyboard.GetEventKey(EKey::Space, EInputState::Pressed).RemoveListener(this, &WGameStateButton::OnKeyboardSpaceKeyPressed);
    GetScene()->GetEventGameStateUpdated().RemoveListener(this, &Self::OnGameStateChanged);
}

void WGameStateButton::OnKeyboardSpaceKeyPressed() const
{
    SwitchPlayPause();
}

void WGameStateButton::OnGameStateChanged(EGameState gameState) const
{
    playPauseBg_->SetColor(GetScene()->GetIsGamePlaying()
        ? Colors::White.WithValue(0.5f)
        : Colors::Green
    );
    stopBg_->SetColor(GetScene()->GetIsGameStopped()
        ? Colors::Red.WithAlpha(0.1f)
        : Colors::Red
    );
}

void WGameStateButton::SwitchPlayPause() const
{
    if (GetScene()->GetIsGamePlaying())
    {
        GetScene()->StopGame();
    }
    else
    {
        GetScene()->PlayGame();
    }
}

#include "generated/GameStateButton.generated.inl"
