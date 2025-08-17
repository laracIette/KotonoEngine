#pragma once
class RInterfaceImageObject;
class RInterfaceTextBoxObject;
class RInterfaceFloatTextBoxObject;
class RInterfaceWindowObject;

class SInterface final
{
	friend class SEngine;

private:
	void Init();

public:
	RInterfaceWindowObject* viewport_;

private:
	RInterfaceImageObject* image1_;

	//temp
	void OnTextBox2ValueChanged(const float delta) const;
	void OnTextBox3ValueChanged(const float delta) const;

	void Test() const;
};

