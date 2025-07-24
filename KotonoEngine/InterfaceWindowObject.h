#pragma once
#include "InterfaceObject.h"
class KInterfaceTextComponent;
class RInterfaceWindowObject : public RInterfaceObject
{
	BASECLASS(RInterfaceObject)

public:
	RInterfaceWindowObject(const std::string_view displayName);

protected:
	void Init() override;
	
public:
	const std::string& GetDisplayName() const;

private:
	const std::string displayName_;

	KInterfaceTextComponent* displayNameTextComponent_;
};

