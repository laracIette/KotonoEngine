#pragma once
#include <kotono_common/Set.h>
#include "kotono_object/Ptr.h"
class WButton;
class GInterface final
{
public:
	void Init();

	void AddButton(UPtr<WButton> const& button);
	void RemoveButton(UPtr<WButton> const& button);

private:
	void OnMouseLeftButtonPressed() const;
	void OnMouseLeftButtonReleased() const;

private:
	USet<UPtr<WButton>> buttons_;
};

inline GInterface Interface;
