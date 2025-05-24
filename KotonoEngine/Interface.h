#pragma once
#include "Object.h"

class RInterfaceImageObject;
class RInterfaceTextBoxObject;

class KInterface : public KObject
{
	BASECLASS(KObject)

public:
	void Construct() override;
	void Init() override;

	void SetLayout() const;

private:
	RInterfaceImageObject* image1_;
	RInterfaceImageObject* image2_;
	RInterfaceTextBoxObject* textBox_;
};

