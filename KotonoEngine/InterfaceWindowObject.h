#pragma once
#include "InterfaceObject.h"
class RInterfaceWindowObject : public RInterfaceObject
{
	BASECLASS(RInterfaceObject)

public:
	RInterfaceWindowObject(const std::string_view displayName);

	const std::string& GetDisplayName() const;

private:
	const std::string displayName_;
};

