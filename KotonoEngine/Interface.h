#pragma once
class RInterfaceImageObject;
class RInterfaceTextBoxObject;
class RInterfaceFloatTextBoxObject;
class RInterfaceWindowObject;

class SInterface final
{
public:
	void Init();

	RInterfaceWindowObject* viewport_;

private:
	RInterfaceImageObject* image1_;
	RInterfaceImageObject* image2_;
	RInterfaceTextBoxObject* textBox1_;
	RInterfaceFloatTextBoxObject* textBox2_;
	RInterfaceFloatTextBoxObject* textBox3_;

	//temp
	void OnTextBox2ValueChanged(const float delta) const;
	void OnTextBox3ValueChanged(const float delta) const;
};

