#pragma once
#include "Object.h"

class RInterfaceImageObject;
class RInterfaceTextBoxObject;
class RInterfaceFloatTextBoxObject;

class KInterface : public KObject
{
	BASECLASS(KObject)

public:
	void Construct() override;
	void Init() override;

	void SetLayout();

private:
	RInterfaceImageObject* image1_;
	RInterfaceImageObject* image2_;
	RInterfaceTextBoxObject* textBox1_;
	RInterfaceFloatTextBoxObject* textBox2_;
	RInterfaceFloatTextBoxObject* textBox3_;

	//temp
	void OnTextBox2ValueChanged(float delta);
	void OnTextBox3ValueChanged(float delta);
};

