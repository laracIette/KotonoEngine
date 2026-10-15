#include "SceneVisibilityFieldButtonStyle.h"

KSceneVisibilityFieldButtonStyle::KSceneVisibilityFieldButtonStyle()
{
	NormalState.color *= Colors::Red;
	FocusedState.color *= Colors::Red;
	PressedState.color *= Colors::Red;
	ActivatedNormalState.color *= Colors::Green;
	ActivatedFocusedState.color *= Colors::Green;
	ActivatedPressedState.color *= Colors::Green;
}

#include "SceneVisibilityFieldButtonStyle.generated.inl"
