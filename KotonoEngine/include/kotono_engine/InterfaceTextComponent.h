#pragma once
#include "InterfaceComponent.h"

class KtShader;
class KTimer;
class KInterfaceImageComponent;

class KInterfaceTextComponent : public KInterfaceComponent
{
	BASECLASS(KInterfaceComponent)

private:
	using TextBinding = std::function<const std::string()>;

public:
	KInterfaceTextComponent(RInterfaceObject* owner);

protected:
	void Init() override;
	void Cleanup() override;

public:
	const std::string& GetText() const;
	const float GetFontSize() const;
	const float GetSpacing() const;
	KtShader* GetShader() const;

	void SetText(const std::string_view text);
	void SetFontSize(const float fontSize);
	void SetSpacing(const float spacing);
	void SetShader(KtShader* shader);
	void SetTextBinding(const TextBinding& function);

	void UpdateTextWithBinding();

private:
	std::string text_;
	KtPool<KInterfaceImageComponent*> characters_;
	float fontSize_;
	float spacing_;

	KtShader* shader_;

	TextBinding textBinding_;
	void UpdateCharacters();
};

