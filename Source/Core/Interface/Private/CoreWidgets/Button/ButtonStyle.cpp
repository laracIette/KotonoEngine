#include "CoreWidgets/Button/ButtonStyle.h"

static const UPath DEFAULT_TEXTURE{ "${ENGINE_DIRECTORY}/Assets/textures/white_texture.jpg" };

KButtonStyle::KButtonStyle()
	: DisabledState{ EImageType::Normal,			DEFAULT_TEXTURE, Colors::White.WithValue(0.05f),	UCorners::All(8.0f) }
	, NormalState{ EImageType::Normal,				DEFAULT_TEXTURE, Colors::White.WithValue(0.1f),	UCorners::All(8.0f) }
	, FocusedState{	EImageType::Normal,				DEFAULT_TEXTURE, Colors::White.WithValue(0.15f),	UCorners::All(8.0f) }
	, PressedState{	EImageType::Normal,				DEFAULT_TEXTURE, Colors::White.WithValue(0.25f),	UCorners::All(8.0f) }
	, ActivatedNormalState{	EImageType::Normal,		DEFAULT_TEXTURE, Colors::White.WithValue(0.2f),	UCorners::All(8.0f) }
	, ActivatedFocusedState{ EImageType::Normal,	DEFAULT_TEXTURE, Colors::White.WithValue(0.15f),	UCorners::All(8.0f) }
	, ActivatedPressedState{ EImageType::Normal,	DEFAULT_TEXTURE, Colors::White.WithValue(0.25f),	UCorners::All(8.0f) }
	, HasShadow{ false }
	, ShadowColor{ 0.01f }
{
}

#include "ButtonStyle.generated.inl"
