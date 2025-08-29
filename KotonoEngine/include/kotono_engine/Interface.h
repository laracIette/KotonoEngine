#pragma once
class RInterfaceLayoutObject;
class RInterfaceImageObject;
class WWidget;
class SInterface final
{
	friend class SEngine;

private:
	void Init();
	void Cleanup();

private:
	RInterfaceLayoutObject* layout_;
	RInterfaceImageObject* image1_;

	WWidget* widget_;

	//temp
	void OnTextBox2ValueChanged(const float delta) const;
	void OnTextBox3ValueChanged(const float delta) const;
};

