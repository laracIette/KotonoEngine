#pragma once
class RInterfaceLayoutObject;
class RInterfaceImageObject;
class SInterface final
{
	friend class SEngine;

private:
	void Init();

private:
	RInterfaceLayoutObject* layout_;
	RInterfaceImageObject* image1_;

	//temp
	void OnTextBox2ValueChanged(const float delta) const;
	void OnTextBox3ValueChanged(const float delta) const;
};

