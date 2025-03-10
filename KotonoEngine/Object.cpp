#include "Object.h"

const std::string& OObject::GetName() const
{
    return _name;
}

void OObject::SetName(const std::string& name)
{
    _name = name;
}
