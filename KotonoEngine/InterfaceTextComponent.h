#pragma once
#include "InterfaceStackComponent.h"

class KtShader;

class KInterfaceTextComponent : public KInterfaceStackComponent
{
	BASECLASS(KInterfaceStackComponent)

public:
	typedef std::function<const std::string()> TextBinding;

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
	void SetTextBinding(const TextBinding& function);

private:
	std::string text_;
	float fontSize_;

	KtShader* shader_;

	TextBinding textBinding_;

	void UpdateTextWithBinding();
	void UpdateCharacters();
};

