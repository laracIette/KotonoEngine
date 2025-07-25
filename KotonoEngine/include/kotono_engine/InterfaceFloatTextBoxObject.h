#pragma once
#include "InterfaceTextBoxObject.h"

class KTimer;

class RInterfaceFloatTextBoxObject : public RInterfaceTextBoxObject
{
	BASECLASS(RInterfaceTextBoxObject)

public:
	RInterfaceFloatTextBoxObject();

protected:
	void Init() override;

public:
	const float GetValue() const;
	KtEvent<float>& GetEventValueChanged();

	void SetValue(const float value);

protected:
	void OnMouseDown() override;

private:
	float value_;

	KtEvent<float> eventValueChanged_;
};

