#include "MaterialNodeInput.h"

MaterialNodeInput::MaterialNodeInput(const std::string& name, const std::string& type, const std::string& value) :
	_name(name), 
	_type(type), 
	_value(value), 
	_source("")
{
}

const std::string& MaterialNodeInput::GetName() const
{
	return _name;
}

const std::string& MaterialNodeInput::GetType() const
{
	return _type;
}

const std::string& MaterialNodeInput::GetValue() const
{
	return _value;
}

const std::string& MaterialNodeInput::GetSource() const
{
	return _source;
}

void MaterialNodeInput::SetValue(const std::string& value)
{
	_value = value;
}

void MaterialNodeInput::SetType(const std::string& type)
{
	_type = type;
}

void MaterialNodeInput::SetSource(const std::string& source)
{
	_source = source;
}
