#include "InterfaceWindowObject.h"

RInterfaceWindowObject::RInterfaceWindowObject(const std::string_view displayName) :
	Base(),
	displayName_(displayName)
{
}

const std::string& RInterfaceWindowObject::GetDisplayName() const
{
	return displayName_;
}
