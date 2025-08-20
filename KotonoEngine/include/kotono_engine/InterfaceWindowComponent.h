#pragma once
#include "InterfaceComponent.h"
class RInterfaceObject;
class KInterfaceTextComponent;
class KInterfaceBoxComponent;
class KInterfaceWindowComponent : public KInterfaceComponent
{
	BASECLASS(KInterfaceComponent)

public:
	KInterfaceWindowComponent(RInterfaceObject* owner);

protected:
	void Init() override;
	
public:
	const std::string& GetDisplayName() const;

private:
	std::string displayName_;

	KInterfaceTextComponent* displayNameTextComponent_;
	KInterfaceBoxComponent* backgroundComponent_;
};

