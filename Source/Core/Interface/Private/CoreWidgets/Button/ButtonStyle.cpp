#include "CoreWidgets/Button/ButtonStyle.h"

static const UPath DEFAULT_TEXTURE{ "${ENGINE_DIRECTORY}/Assets/textures/white_texture.jpg" };

KButtonStyle::KButtonStyle()
	: DisabledState{ DEFAULT_TEXTURE,			Colors::White.WithValue(0.05f) }
	, NormalState{	DEFAULT_TEXTURE,			Colors::White.WithValue(0.1f) }
	, FocusedState{	DEFAULT_TEXTURE,			Colors::White.WithValue(0.15f) }
	, PressedState{	DEFAULT_TEXTURE,			Colors::White.WithValue(0.25f) }
	, ActivatedNormalState{	DEFAULT_TEXTURE,	Colors::White.WithValue(0.2f) }
	, ActivatedFocusedState{ DEFAULT_TEXTURE,	Colors::White.WithValue(0.15f) }
	, ActivatedPressedState{ DEFAULT_TEXTURE,	Colors::White.WithValue(0.25f) }
{
}

#include "ButtonStyle.generated.inl"
