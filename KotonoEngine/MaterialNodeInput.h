#pragma once
#include <string>
class MaterialNodeInput
{
public:
	MaterialNodeInput(const std::string& name, const std::string& type, const std::string& value);

	const std::string& GetName() const;
	const std::string& GetType() const;
	const std::string& GetValue() const;
	const std::string& GetSource() const;

	void SetType(const std::string& type);
	void SetValue(const std::string& value);
	void SetSource(const std::string& source);

private:
	const std::string _name;
	std::string _type;
	std::string _value;
	std::string _source;
};

