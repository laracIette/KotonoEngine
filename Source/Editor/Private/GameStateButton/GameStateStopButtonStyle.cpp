#include "GameStateStopButtonStyle.h"

KGameStateStopButtonStyle::KGameStateStopButtonStyle()
{
	NormalState.color = Colors::Red;
	DisabledState.color = Colors::Red.WithValue(0.2f);
}

#include "GameStateStopButtonStyle.generated.inl"
