#pragma once
#include "InterfaceStackComponent.h"

class KtShader;
class KTimer;

class KInterfaceTextComponent : public KInterfaceStackComponent
{
	BASECLASS(KInterfaceStackComponent)

private:
	using TextBinding = std::function<const std::string()>;

public:
	KInterfaceTextComponent(RInterfaceObject* owner);

	void Init() override;
	void Cleanup() override;

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
	KTimer* updateTextWithBindingTimer_;

	KtShader* shader_;

	TextBinding textBinding_;

	void UpdateTextWithBinding();
	void UpdateCharacters();
};

