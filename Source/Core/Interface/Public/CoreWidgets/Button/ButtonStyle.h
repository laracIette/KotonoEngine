#pragma once
#include "Widget/WidgetStyle.h"

#include <Color.h>
#include <Corners.h>

#include "ButtonStyle.generated.h"

class KButtonStyle : public KWidgetStyle
{
	GENERATED()
	
public:
	enum class EImageType : u8
	{
		None,
		Normal,
		Rounded,
	};

	struct State
	{
		EImageType imageType;
		UPath texture;
		UColor color;
		UCorners corners;
	};
	
public:
	KButtonStyle();
	
private:
	ProtectedProperty(State, DisabledState);
	ProtectedProperty(State, NormalState);
	ProtectedProperty(State, FocusedState);
	ProtectedProperty(State, PressedState);
	ProtectedProperty(State, ActivatedNormalState);
	ProtectedProperty(State, ActivatedFocusedState);
	ProtectedProperty(State, ActivatedPressedState);
	ProtectedProperty(b8, HasShadow);
	ProtectedProperty(glm::vec2, ShadowOffset);
	ProtectedProperty(UColor, ShadowColor);
};
