#pragma once
#include "InterfaceObject.h"
class RText : public RInterfaceObject
{
	BASECLASS(RInterfaceObject)

public:
	const std::string& GetText() const;

	void SetText(const std::string& text);

private:
	std::string text_;
	float maxWidth_;

	void UpdateTexture();
};

