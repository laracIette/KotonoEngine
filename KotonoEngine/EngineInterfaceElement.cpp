#include "EngineInterfaceElement.h"

const int EngineInterfaceElement::GetLayer() const
{
	return _parent ? _parent->GetLayer() + 1 : 0;
}

const bool EngineInterfaceElement::IsHovered() const
{
	return false; // TODO: hovered
}

