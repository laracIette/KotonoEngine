#pragma once
#include <vector>
class WButton;
class SInterface final
{
public:
	void Init();

	void AddButton(WButton* button);
	void RemoveButton(WButton* button);

private:
	void OnMouseLeftButtonPressed();
	void OnMouseLeftButtonReleased();

private:
	std::vector<WButton*> buttons_;
};

inline SInterface Interface;
