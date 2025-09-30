#pragma once
class RInterfaceLayoutObject;
class RInterfaceImageObject;
class SInterface final
{
	friend class SEngine;

private:
	void Init();
	void Cleanup();

private:
	RInterfaceLayoutObject* layout_;
};

