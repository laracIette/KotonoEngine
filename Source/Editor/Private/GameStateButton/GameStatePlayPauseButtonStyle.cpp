#include "GameStatePlayPauseButtonStyle.h"

KGameStatePlayPauseButtonStyle::KGameStatePlayPauseButtonStyle()
{
	NormalState.color = Colors::Green;
	ActivatedState.color = Colors::White.WithValue(0.5f);
}

#include "GameStatePlayPauseButtonStyle.generated.inl"
