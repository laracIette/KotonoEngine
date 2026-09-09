#include "GameStateButton.h"

#include <kotono_core/Scene.h>
#include <kotono_interface/widgets.h>

static constexpr UColor PLAYING_COLOR{ Colors::Green };
static constexpr UColor PAUSE_COLOR{ Colors::White.WithValue(0.5f) };
static constexpr UColor STOP_COLOR{ Colors::Red };
static constexpr UColor STOPPED_COLOR{ Colors::Red.WithValue(0.2f) };

WidgetPtr WGameStateButton::Build()
{
    playPauseBg_ = UCreate<WColor>{}();
    playPauseBg_->SetColor(GetScene()->GetIsGamePlaying()
        ? PAUSE_COLOR
        : PLAYING_COLOR
    );

    UPtr playPauseButton{ UCreate<WButton>{}() };
    playPauseButton->SetOnPressed([this]() { SwitchPlayPause(); });
    playPauseButton->SetIsVisible(false);

    stopBg_ = UCreate<WColor>{}();
    stopBg_->SetColor(GetScene()->GetIsGameStopped()
        ? STOPPED_COLOR
        : STOP_COLOR
    );

    UPtr stopButton{ UCreate<WButton>{}() };
    stopButton->SetOnPressed([this]() {
        if (!GetScene()->GetIsGameStopped())
        {
            GetScene()->StopGame();
        }
    });
    stopButton->SetIsVisible(false);

    auto const widgetTree{ UChildrenOwnerTree{ UCreate<WRow>{ "Main Row" }(5.0f), {
        new UChildOwnerTree{ UCreate<WBox>{ "Play Pause Box" }(glm::vec2{ 64.0f, 64.0f }),
            new UChildrenOwnerTree{ UCreate<WStack>{}(), {
                new UWidgetTreeLeaf{ playPauseBg_ },
                new UWidgetTreeLeaf{ playPauseButton },
            } }
        },
        new UChildOwnerTree{ UCreate<WBox>{ "Stop Box" }(glm::vec2{ 64.0f, 64.0f }),
            new UChildrenOwnerTree{ UCreate<WStack>{}(), {
                new UWidgetTreeLeaf{ stopBg_ },
                new UWidgetTreeLeaf{ stopButton },
            } }
        },
    } } };
    widgetTree.Link();

    return widgetTree.Widget();
}

void WGameStateButton::Display(UWidgetDisplaySettings const& displaySettings)
{
    Base::Display(displaySettings);

    GetScene()->GetEventGameStateUpdated().AddListener(this, &Self::OnGameStateChanged);
}

void WGameStateButton::Remove()
{
    Base::Remove();

    GetScene()->GetEventGameStateUpdated().RemoveListener(this, &Self::OnGameStateChanged);
}

void WGameStateButton::OnGameStateChanged(EGameState gameState) const
{
    playPauseBg_->SetColor(GetScene()->GetIsGamePlaying()
        ? PAUSE_COLOR
        : PLAYING_COLOR
    );
    stopBg_->SetColor(GetScene()->GetIsGameStopped()
        ? STOPPED_COLOR
        : STOP_COLOR
    );
}

void WGameStateButton::SwitchPlayPause() const
{
    if (GetScene()->GetIsGamePlaying())
    {
        GetScene()->PauseGame();
    }
    else
    {
        GetScene()->PlayGame();
    }
}

#include "generated/GameStateButton.generated.inl"
