#include "GameStateButton.h"

#include <Scene/Scene.h>
#include <core_widgets.h>

static constexpr UColor PLAY_COLOR{ Colors::Green };
static constexpr UColor PAUSE_COLOR{ Colors::White.WithValue(0.5f) };
static constexpr UColor STOP_COLOR{ Colors::Red };
static constexpr UColor STOPPED_COLOR{ Colors::Red.WithValue(0.2f) };

WidgetPtr WGameStateButton::Build()
{
    UPtr const playPauseButton{ UCreate<WButton>{ "Play Pause Button" }() };

    return (
        UCreate<WRow>{ "Main Row" }()
        | Apply(&WRow::SetSpacing, 5.0f)
        | (
            UCreate<WBox>{ "Play Pause Box" }()
            | Apply(&WBox::SetSize, glm::vec2{ 64.0f, 64.0f })
            | (
                playPauseButton
                | Apply(&WButton::SetIsActivatable, true)
                | Apply(&WButton::SetNormalColor, PLAY_COLOR)
                | Apply(&WButton::SetActivatedColor, PAUSE_COLOR)
                | Apply(&WButton::SetOnActivated, [this]() { GetScene()->PlayGame(); })
                | Apply(&WButton::SetOnDeactivated, [this]() { GetScene()->PauseGame(); })
            )
        )
        | (
            UCreate<WBox>{ "Stop Box" }()
            | Apply(&WBox::SetSize, glm::vec2{ 64.0f, 64.0f })
            | (
                UCreate<WButton>{ "Play Pause Button" }()
                | Apply(&WButton::SetIsEnabled, [this]() { return !GetScene()->GetIsGameStopped(); })
                | Apply(&WButton::SetNormalColor, STOP_COLOR)
                | Apply(&WButton::SetDisabledColor, STOPPED_COLOR)
                | Apply(&WButton::SetOnClicked, [this, playPauseButton]() {
                    GetScene()->StopGame();
                    playPauseButton->SetIsActivated(false);
                })
            )
        )
    );
}

#include "GameStateButton.generated.inl"
