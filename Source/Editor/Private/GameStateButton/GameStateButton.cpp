#include "GameStateButton.h"

#include "GameStatePlayPauseButtonStyle.h"
#include "GameStateStopButtonStyle.h"
#include <Scene/Scene.h>
#include <core_widgets.h>

WidgetPtr WGameStateButton::Build()
{
    UObjectPtr const playPauseButton{ UCreate<WButton>{ "Play Pause Button" }() };

    return (
        UCreate<WRow>{ "Main Row" }()
        | Apply(&WRow::SetSpacing, 5.0f)
        | (
            UCreate<WBox>{ "Play Pause Box" }()
            | Apply(&WBox::SetSize, glm::vec2{ 64.0f, 64.0f })
            | (
                playPauseButton
                | Apply(&WButton::SetStyleClass, KGameStatePlayPauseButtonStyle::Class::Get())
                | Apply(&WButton::SetIsActivatable, true)
                | Apply(&WButton::SetOnActivated, [this]() { GetScene()->PlayGame(); })
                | Apply(&WButton::SetOnDeactivated, [this]() { GetScene()->PauseGame(); })
            )
        )
        | (
            UCreate<WBox>{ "Stop Box" }()
            | Apply(&WBox::SetSize, glm::vec2{ 64.0f, 64.0f })
            | (
                UCreate<WButton>{ "Play Pause Button" }()
                | Apply(&WButton::SetStyleClass, KGameStateStopButtonStyle::Class::Get())
                | Apply(&WButton::SetIsEnabled, [this]() { return !GetScene()->GetIsGameStopped(); })
                | Apply(&WButton::SetOnClicked, [this, playPauseButton]() {
                    GetScene()->StopGame();
                    playPauseButton->SetIsActivated(false);
                })
            )
        )
    );
}

#include "GameStateButton.generated.inl"
