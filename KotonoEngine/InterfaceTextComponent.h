#pragma once
#include "InterfaceComponent.h"

class KtShader;
class KInterfaceStackComponent;
class KTimer;

class KInterfaceTextComponent : public KInterfaceComponent
{
	BASECLASS(KInterfaceComponent)

public:
	typedef std::function<const std::string()> TextBinding;

	void Construct() override;
	void Init() override;

	const std::string& GetText() const;
	const float GetFontSize() const;
	const float GetSpacing() const;
	KtShader* GetShader() const;
	KTimer* GetUpdateTimer() const;

	void SetText(const std::string& text);
	void SetFontSize(const float fontSize);
	void SetSpacing(const float spacing);
	void SetShader(KtShader* shader);
	void SetTextBinding(const TextBinding& function);

private:
	KInterfaceStackComponent* characters_;

	std::string text_;
	float fontSize_;

	KtShader* shader_;

	TextBinding textBinding_;
	// Timer to avoid pre-initialization delete, also helps with performance
	KTimer* updateTextTimer_;

	void UpdateTextWithBinding();
	void UpdateCharacters();
};

