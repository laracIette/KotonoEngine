#pragma once
#include "InterfaceObject.h"

class KtShader;
class RInterfaceObjectStack;

class RText : public RInterfaceObject
{
	BASECLASS(RInterfaceObject)

public:
	void Construct() override;
	void Init() override;

	const std::string& GetText() const; 
	const float GetFontSize() const;
	const float GetSpacing() const;
	KtShader* GetShader() const;

	void SetText(const std::string& text);
	void SetFontSize(const float fontSize);
	void SetSpacing(const float spacing);
	void SetShader(KtShader* shader);

private:
	RInterfaceObjectStack* letters_;

	std::string text_;
	float fontSize_;

	KtShader* shader_;

	void UpdateLetters();
};

