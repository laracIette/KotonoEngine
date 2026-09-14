#include "GameStateButton.h"

#include <kotono_core/Scene.h>
#include <kotono_interface/widgets.h>

static constexpr UColor PLAY_COLOR{ Colors::Green };
static constexpr UColor PAUSE_COLOR{ Colors::White.WithValue(0.5f) };
static constexpr UColor STOP_COLOR{ Colors::Red };
static constexpr UColor STOPPED_COLOR{ Colors::Red.WithValue(0.2f) };

WidgetPtr WGameStateButton::Build()
{
    playPauseButton_ = UCreate<WButton>{ "Play Pause Button" }();
    playPauseButton_->SetNormalColor(PLAY_COLOR);
    playPauseButton_->SetFocusedColor(playPauseButton_->GetNormalColor() * 0.9f);
    playPauseButton_->SetOnClicked([this]() {
        if (GetScene()->GetIsGamePlaying())
        {
            GetScene()->PauseGame();
        }
        else
        {
            GetScene()->PlayGame();
        }
    });

    stopButton_ = UCreate<WButton>{ "Play Pause Button" }();
    stopButton_->SetNormalColor(STOP_COLOR);
    stopButton_->SetFocusedColor(stopButton_->GetNormalColor() * 0.9f);
    stopButton_->SetDisabledColor(STOPPED_COLOR);
    stopButton_->SetOnClicked([this]() { GetScene()->StopGame(); });
    stopButton_->SetIsEnabled(!GetScene()->GetIsGameStopped());

    auto const widgetTree{ UChildrenOwnerTree{ UCreate<WRow>{ "Main Row" }(5.0f), {
        new UChildOwnerTree{ UCreate<WBox>{ "Play Pause Box" }(glm::vec2{ 64.0f, 64.0f }),
            new UWidgetTreeLeaf{ playPauseButton_ }
        },
        new UChildOwnerTree{ UCreate<WBox>{ "Stop Box" }(glm::vec2{ 64.0f, 64.0f }),
            new UWidgetTreeLeaf{ stopButton_ },
        },
    } } };
    widgetTree.Link();

    return widgetTree.Widget();
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
