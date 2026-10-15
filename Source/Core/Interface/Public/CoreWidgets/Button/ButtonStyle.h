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
	ProtectedProperty(State, NormalState);
	ProtectedProperty(State, FocusedState);
	ProtectedProperty(State, PressedState);
	ProtectedProperty(State, ActivatedState);
	ProtectedProperty(State, DisabledState);
};
