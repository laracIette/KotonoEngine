#pragma once
#include "Widget/WidgetStyle.h"

#include <Color.h>

#include "ButtonStyle.generated.h"

class KButtonStyle : public KWidgetStyle
{
	GENERATED()
	
public:
	struct State
	{
		UPath texture;
		UColor color;
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
};
