#pragma once
#include <Windows.h>
#include <string>
class UGuid
{
public:
	UGuid();

	operator std::string() const;
	void operator=(const std::string& string);

	bool operator==(const UGuid& other) const;

private:
	GUID _guid;
};

