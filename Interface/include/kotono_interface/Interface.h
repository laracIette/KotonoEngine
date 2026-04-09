#pragma once
#include <kotono_common/Pool.h>
template <typename T>
class UPtr;
class WButton;
class SInterface final
{
public:
	void Init();

	void AddButton(const UPtr<WButton>& button);
	void RemoveButton(const UPtr<WButton>& button);

private:
	void OnMouseLeftButtonPressed() const;
	void OnMouseLeftButtonReleased() const;

private:
	KtPool<UPtr<WButton>> buttons_;
};

inline SInterface Interface;
