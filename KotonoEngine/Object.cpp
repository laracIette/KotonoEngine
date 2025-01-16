#include "Object.h"

Object::Object()
{
}

const std::type_info& Object::GetType() const
{
    return typeid(*this);
}

const std::string& Object::GetName() const
{
    return _name;
}

void Object::SetName(const std::string& name)
{
    _name = name;
}

const std::string Object::ToString() const
{
    return std::format("{}, Name: {}, Type: {}", static_cast<const void*>(this), _name, GetType().name());
}
