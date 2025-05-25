#pragma once
#include "InterfaceTextBoxObject.h"
class RInterfaceFloatTextBoxObject : public RInterfaceTextBoxObject
{
	BASECLASS(RInterfaceTextBoxObject)

public:
	void Init() override;

	const float GetValue() const;
	KtEvent<float>& GetValueChangedEvent();

	void SetValue(const float value);

protected:
	void OnMouseDown() override;

private:
	float value_;

	KtEvent<float> valueChangedEvent_;
};

