#include "MaterialNodeOperation.h"
#include <format>
#include <sstream>

template <char __operator>
MaterialNodeOperation<__operator>::MaterialNodeOperation() :
    MaterialNode(__operator),
    _type("float")
{
    AddInput();
    AddInput();
}

template <char __operator>
const std::string MaterialNodeOperation<__operator>::GetDefaultValue() const
{
    switch (_type)
    {
    case "int":
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
const std::string MaterialNodeOperation<__operator>::GetShaderCode() const
{
    std::stringstream stream;
    stream << _type << ' ' << Id << " = " << Inputs[0]->GetValue();
    for (size_t i = 1; i < Inputs.size(); i++)
    {
        stream << ' ' << __operator << ' ' << Inputs[i]->GetValue();
    }
    stream << ';';
    return stream.str();
}

template<char __operator>
void MaterialNodeOperation<__operator>::AddInput()
{
    std::string name = std::format("[{}]", Inputs.size());
    Inputs.push_back(new MaterialNodeInput(name, _type, GetDefaultValue()));
}
