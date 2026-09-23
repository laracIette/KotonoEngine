#include "GameStateButton.h"

#include <kotono_core/Scene.h>
#include <kotono_interface/widgets.h>

static constexpr UColor PLAY_COLOR{ Colors::Green };
static constexpr UColor PAUSE_COLOR{ Colors::White.WithValue(0.5f) };
static constexpr UColor STOP_COLOR{ Colors::Red };
static constexpr UColor STOPPED_COLOR{ Colors::Red.WithValue(0.2f) };

WidgetPtr WGameStateButton::Build()
{
    return (
        UCreate<WRow>{ "Main Row" }()
        | Apply(&WRow::SetSpacing, 5.0f)
        | (
            UCreate<WBox>{ "Play Pause Box" }()
            | Apply(&WBox::SetSize, glm::vec2{ 64.0f, 64.0f })
            | (
                playPauseButton_ = UCreate<WButton>{ "Play Pause Button" }()
                | Apply(&WButton::SetNormalColor, PLAY_COLOR)
                | Apply(&WButton::SetFocusedColor, PLAY_COLOR * 0.9f)
                | Apply(&WButton::SetOnClicked, [this]() {
                    if (GetScene()->GetIsGamePlaying())
                    {
                        GetScene()->PauseGame();
                    }
                    else
                    {
                        GetScene()->PlayGame();
                    }
                })
            )
        )
        | (
            UCreate<WBox>{ "Stop Box" }()
            | Apply(&WBox::SetSize, glm::vec2{ 64.0f, 64.0f })
            | (
                stopButton_ = UCreate<WButton>{ "Play Pause Button" }()
                | Apply(&WButton::SetNormalColor, STOP_COLOR)
                | Apply(&WButton::SetFocusedColor, STOP_COLOR * 0.9f)
                | Apply(&WButton::SetDisabledColor, STOPPED_COLOR)
                | Apply(&WButton::SetOnClicked, [this]() { GetScene()->StopGame(); })
                | Apply(&WButton::SetIsEnabled, !GetScene()->GetIsGameStopped())
            )
        )
    );
}

void WGameStateButton::Display(UWidgetDisplaySettings const& displaySettings)
{
    Base::Display(displaySettings);

    GetScene()->GetEventGameStateChanged().AddListener(this, &Self::OnGameStateChanged);
}

void WGameStateButton::Remove()
{
    Base::Remove();

    GetScene()->GetEventGameStateChanged().RemoveListener(this, &Self::OnGameStateChanged);
}

void WGameStateButton::OnGameStateChanged(EGameState gameState) const
{
    if (playPauseButton_)
    {
        playPauseButton_->SetNormalColor(gameState == EGameState::Playing ? PAUSE_COLOR : PLAY_COLOR);
        playPauseButton_->SetFocusedColor(playPauseButton_->GetNormalColor() * 0.9f);
    }
    if (stopButton_)
    {
        stopButton_->SetIsEnabled(gameState != EGameState::Stopped);
    }
}

#include "generated/GameStateButton.generated.inl"
