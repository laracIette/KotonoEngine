#include "MaterialNodeOperation.h"
#include <format>
#include <sstream>

template <char __operator>
MaterialNodeOperation<__operator>::MaterialNodeOperation() :
    MaterialNode(__operator),
    _type("float")
{
    AddInput();
}

template <char __operator>
std::string MaterialNodeOperation<__operator>::GetDefaultValue() const
{
    switch (_type)
    {
    case "int":
        return "0";
    case "long":
        return "0";
    case "float": 
        return "0.0f";
    case "double": 
        return "0.0";
    default: 
        return "0";
    }
}

template <char __operator>
std::string MaterialNodeOperation<__operator>::GetShaderCode() const
{
    std::stringstream stream;
    stream << _inputs[0]->GetValue();
    for (size_t i = 1; i < _inputs.size(); i++)
    {
        stream << ' ' << __operator << ' ' << _inputs[i]->GetValue();
    }
    return stream.str();
}

template<char __operator>
void MaterialNodeOperation<__operator>::AddInput()
{
    std::string name = std::format("[{}]", _inputs.size());
    _inputs.push_back(new MaterialNodeInput(name, _type, GetDefaultValue()));
}
