#pragma once
#include <kotono_common/Pool.h>
#include <unordered_set>
class WButton;
class SInterface final
{
public:
	void Init();

	void AddButton(WButton* button);
	void RemoveButton(WButton* button);

private:
	void RemoveButtons();

	void OnMouseLeftButtonPressed();
	void OnMouseLeftButtonReleased();

private:
	KtPool<WButton*> buttons_;
	std::unordered_set<WButton*> removes_;
};

inline SInterface Interface;
